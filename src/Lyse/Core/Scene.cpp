///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

#include "Shaders/shaders.cpp"

namespace lys
{
	namespace
	{
		#pragma pack(push, 1)
		struct alignas(16) UboLightData
		{
			alignas(4) uint32_t type;
			alignas(4) uint32_t shadowMapStartIndex;
			alignas(4) uint32_t shadowMapStopIndex;

			alignas(16) scp::f32vec3 color;

			alignas(16) scp::f32vec4 param0;
			alignas(16) scp::f32vec4 param1;
		};

		struct alignas(16) UboLightsData
		{
			alignas(4) uint32_t count;
			alignas(16) UboLightData lights[SceneParameters::maxLightSlotCount];
		};

		struct alignas(16) UboDrawableData
		{
			alignas(16) scp::f32mat4x4 model;
			alignas(16) scp::f32mat4x4 invModel;
			alignas(16) scp::f32mat4x4 viewModel;
			alignas(16) scp::f32mat4x4 invViewModel;
			alignas(16) scp::f32mat4x4 projectionViewModel;
			alignas(16) scp::f32mat4x4 invProjectionViewModel;
		};

		struct alignas(16) UboShadowCameraData
		{
			alignas(16) scp::f32mat4x4 projection;
			alignas(16) scp::f32mat4x4 view;
			alignas(4) float near;
			alignas(4) float far;
		};

		struct alignas(16) UboShadowCamerasData
		{
			alignas(4) uint32_t count;
			alignas(16) UboShadowCameraData cameras[SceneParameters::maxShadowTextureCount];
		};
		#pragma pack(pop)
	}

	Scene::Scene(uint32_t width, uint32_t height, const SceneParameters params) :
		_screenVao(),
		_screenVbo(),


		_resolution(width, height),
		_params(params),


		_shaders(),
		_shaderMap(),


		_clearColor(0.f, 0.f, 0.f),
		_background(nullptr),

		_camera(nullptr),

		_lights(),
		_uboLights(),

		_drawables(),
		_uboDrawable(sizeof(UboDrawableData), spl::BufferStorageFlags::DynamicStorage),


		_depthTexture(),
		_colorTexture(),
		_materialTexture(),
		_normalTexture(),
		_tangentTexture(),
		_gBufferFramebuffer(),

		_shadowTexture(),
		_shadowMappingFramebuffer(),
		_uboShadowCameras(),

		_transparencyColorTexture(),
		_transparencyCounterTexture(),
		_transparencyFramebuffer(),

		_ssaoTexture(),
		_ssaoFramebuffer(),

		_mergeTexture(),
		_mergeFramebuffer()
	{
		// Create "screen" mesh

		_screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
		_screenVao.setAttributeEnabled(0, true);
		_screenVao.setAttributeBinding(0, 0);
		_screenVao.setBindingDivisor(0, 0);

		static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
		_screenVbo.createNew(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);
		_screenVao.bindArrayBuffer(&_screenVbo, 0, sizeof(float) * 2);

		// Check geometry pass parameters

		// TODO: Check implementation dependent limits (UBOs, textures sizes, etc...)
		assert(_resolution.x > 0 && _resolution.y > 0);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.depthTextureFormat) == spl::TextureFormat::DepthComponent);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.colorTextureFormat) == spl::TextureFormat::RGB);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.materialTextureFormat) == spl::TextureFormat::RGB);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.normalTextureFormat) == spl::TextureFormat::RGB);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.tangentTextureFormat) == spl::TextureFormat::RGB);

		// Check shadow mapping parameters

		if (_params.shadowEnabled)
		{
			assert(_params.shadowTextureCount > 0 && _params.shadowTextureCount <= SceneParameters::maxShadowTextureCount);
			assert(_params.shadowTextureResolution.x > 0 && _params.shadowTextureResolution.y > 0);
			assert(spl::_spl::textureInternalFormatToTextureFormat(_params.shadowTextureFormat) == spl::TextureFormat::DepthComponent);

			spl::TextureCreationParams shadowTextureCreationParams;
			shadowTextureCreationParams.target = spl::TextureTarget::Array2D;
			shadowTextureCreationParams.internalFormat = _params.shadowTextureFormat;
			shadowTextureCreationParams.width = _params.shadowTextureResolution.x;
			shadowTextureCreationParams.height = _params.shadowTextureResolution.y;
			shadowTextureCreationParams.depth = _params.shadowTextureCount;
			_shadowTexture.createNew(shadowTextureCreationParams);

			_shadowTexture.setBorderColor(1.f, 1.f, 1.f, 1.f);
			_shadowTexture.setCompareMode(spl::TextureCompareMode::RefToTexture);
			_shadowTexture.setCompareFunc(spl::CompareFunc::Greater);
			_shadowTexture.setWrappingS(spl::TextureWrapping::ClampToBorder);
			_shadowTexture.setWrappingT(spl::TextureWrapping::ClampToBorder);

			_uboShadowCameras.createNew(offsetof(UboShadowCamerasData, cameras) + sizeof(UboShadowCameraData) * _params.shadowTextureCount, spl::BufferStorageFlags::DynamicStorage);

			_shadowMappingFramebuffer.attachTexture(spl::FramebufferAttachment::DepthAttachment, 0, &_shadowTexture);
		}
		else
		{
			uint32_t data = 0;
			_uboShadowCameras.createNew(offsetof(UboShadowCamerasData, cameras), spl::BufferStorageFlags::None, &data);
		}

		// Check transparency parameters

		if (_params.transparencyEnabled)
		{
			assert(spl::_spl::textureInternalFormatToTextureFormat(_params.transparencyColorTextureFormat) == spl::TextureFormat::RGBA);
			assert(spl::_spl::textureInternalFormatToTextureFormat(_params.transparencyCounterTextureFormat) == spl::TextureFormat::R);
		}

		// Check ssao parameters

		if (_params.ssaoEnabled)
		{
			assert(_params.ssaoSampleCount > 0);
			assert(spl::_spl::textureInternalFormatToTextureFormat(_params.ssaoTextureFormat) == spl::TextureFormat::R);
		}

		// Check merging pass parameters

		assert(_params.lightSlotCount < SceneParameters::maxLightSlotCount);
		assert(spl::_spl::textureInternalFormatToTextureFormat(_params.renderTextureFormat) == spl::TextureFormat::RGB);

		_uboLights.createNew(offsetof(UboLightsData, lights) + sizeof(UboLightData) * _params.lightSlotCount, spl::BufferStorageFlags::DynamicStorage);

		// Compile shaders and create framebuffers with those parameters

		_loadShaders();

		resize(_resolution.x, _resolution.y);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		assert(width > 0 && height > 0);

		_resolution.x = width;
		_resolution.y = height;

		_depthTexture.createNew(_resolution.x, _resolution.y, _params.depthTextureFormat);
		_colorTexture.createNew(_resolution.x, _resolution.y, _params.colorTextureFormat);
		_materialTexture.createNew(_resolution.x, _resolution.y, _params.materialTextureFormat);
		_normalTexture.createNew(_resolution.x, _resolution.y, _params.normalTextureFormat);
		_tangentTexture.createNew(_resolution.x, _resolution.y, _params.tangentTextureFormat);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::DepthAttachment, 0, &_depthTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_colorTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 1, &_materialTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 2, &_normalTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 3, &_tangentTexture);

		if (_params.transparencyEnabled)
		{
			_transparencyColorTexture.createNew(_resolution.x, _resolution.y, _params.transparencyColorTextureFormat);
			_transparencyCounterTexture.createNew(_resolution.x, _resolution.y, _params.transparencyCounterTextureFormat);
			_transparencyFramebuffer.attachTexture(spl::FramebufferAttachment::DepthAttachment, 0, &_depthTexture);
			_transparencyFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_transparencyColorTexture);
			_transparencyFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 1, &_transparencyCounterTexture);
		}

		if (_params.ssaoEnabled)
		{
			_ssaoTexture.createNew(_resolution.x, _resolution.y, _params.ssaoTextureFormat);
			_ssaoFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_ssaoTexture);
		}

		_mergeTexture.createNew(_resolution.x, _resolution.y, _params.renderTextureFormat);
		_mergeFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_mergeTexture);
	}

	void Scene::setBackgroundFlatColor(float red, float green, float blue)
	{
		assert(red >= 0.f && red <= 1.f);
		assert(green >= 0.f && green <= 1.f);
		assert(blue >= 0.f && blue <= 1.f);

		_clearColor.x = red;
		_clearColor.y = green;
		_clearColor.z = blue;
	}

	void Scene::setBackgroundEquirectangular(const spl::Texture2D* texture)
	{
		_background = texture;
	}

	void Scene::setBackgroundCubemap(const spl::Texture* texture)
	{
		assert(texture->getCreationParams().target == spl::TextureTarget::CubeMap);
		_background = texture;
	}

	void Scene::setCamera(const CameraBase* camera)
	{
		_camera = camera;
	}

	void Scene::addLight(const LightBase* light)
	{
		switch (light->getType())
		{
			case LightType::Point:
				assert(dynamic_cast<const LightPoint*>(light));
				break;
		}

		_lights.insert(light);
	}

	void Scene::removeLight(const LightBase* light)
	{
		_lights.erase(light);
	}
	
	void Scene::addDrawable(const Drawable* drawable)
	{
		switch (drawable->getType())
		{
			case DrawableType::Mesh:
				assert(dynamic_cast<const Mesh<>*>(drawable));
				break;
		}

		_drawables.insert(drawable);
	}

	void Scene::removeDrawable(const Drawable* drawable)
	{
		_drawables.erase(drawable);
	}

	void Scene::render()
	{
		// Save context context - `render` must not appear to modify context state from caller pov

		spl::Context* context = spl::Context::getCurrentContext();
		spl::ContextState savedContextState = context->getState();

		// Set context state for rendering

		context->setIsSeamlessCubeMapFilteringEnabled(true);
		context->setViewport(0, 0, _resolution.x, _resolution.y);
		context->setClearColor(0.f, 0.f, 0.f, 0.f);
		context->setClearDepth(1.0);
		context->setClearStencil(0);
		context->setIsDepthWriteEnabled(true);
		context->setIsDepthTestEnabled(true);
		context->setDepthTestFunc(spl::CompareFunc::Less);
		context->setIsStencilTestEnabled(false);
		context->setIsBlendEnabled(0, false);
		context->setIsBlendEnabled(1, false);
		context->setIsBlendEnabled(2, false);
		context->setIsBlendEnabled(3, false);

		// Update and bind UBOs
		
		std::vector<const CameraBase*> shadowCameras;

		_camera->updateAndBindUbo(0);
		_updateAndBindUboLights(1, shadowCameras);
		// 2 :	DrawableData
		_updateAndBindUboShadowCameras(3, shadowCameras);
		
		// Draw G-Buffer
		
		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_gBufferFramebuffer);
		spl::Framebuffer::clear(true, true, false);
		
		std::multimap<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceGBuffer*>, const Drawable*> gBufferDrawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(&gBufferDrawSequence, drawable, ShaderType::GBuffer);
		}
		
		const spl::ShaderProgram* currentShader = nullptr;
		for (const std::pair<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceGBuffer*>, const Drawable*>& elt : gBufferDrawSequence)
		{
			if (elt.first.first != currentShader)
			{
				currentShader = elt.first.first;
				spl::ShaderProgram::bind(currentShader);
			}

			context->setFaceCullingMode(elt.second->getGeometryFaceCullingMode());
			context->setFaceCullingOrientation(elt.second->getGeometryFaceCullingOrientation());
		
			_updateAndBindUboDrawable(2, _camera, elt.second);

			_setDrawableUniforms(elt.first.first, &elt.first.second->u_drawable, elt.second);

			elt.second->_draw();
		}
		
		// Draw shadow maps
		
		if (_params.shadowEnabled)
		{
			context->setViewport(0, 0, _params.shadowTextureResolution.x, _params.shadowTextureResolution.y);

			spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_shadowMappingFramebuffer);
			spl::Framebuffer::clear(false, true, false);

			std::multimap<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceShadow*>, const Drawable*> shadowMappingDrawSequence;
			for (const Drawable* drawable : _drawables)
			{
				_insertInDrawSequence(&shadowMappingDrawSequence, drawable, ShaderType::ShadowMapping);
			}

			const spl::ShaderProgram* currentShader = nullptr;
			for (const std::pair<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceShadow*>, const Drawable*>& elt : shadowMappingDrawSequence)
			{
				if (elt.first.first != currentShader)
				{
					currentShader = elt.first.first;
					spl::ShaderProgram::bind(currentShader);

					_setGBufferResultUniforms(elt.first.first, &elt.first.second->u_gBuffer);
				}

				context->setFaceCullingMode(elt.second->getShadowFaceCullingMode());
				context->setFaceCullingOrientation(elt.second->getShadowFaceCullingOrientation());

				_updateAndBindUboDrawable(2, _camera, elt.second);

				_setDrawableUniforms(elt.first.first, &elt.first.second->u_drawable, elt.second);

				elt.second->_draw();
			}

			context->setViewport(0, 0, _resolution.x, _resolution.y);
		}

		// Draw transparent objects

		context->setIsDepthWriteEnabled(false);

		if (_params.transparencyEnabled)
		{
			context->setIsBlendEnabled(0, true);
			context->setBlendEquations(0, spl::BlendEquation::Add, spl::BlendEquation::Add);
			context->setBlendFuncs(0, spl::BlendFunc::One, spl::BlendFunc::One, spl::BlendFunc::One, spl::BlendFunc::One);

			context->setIsBlendEnabled(1, true);
			context->setBlendEquations(1, spl::BlendEquation::Add, spl::BlendEquation::Add);
			context->setBlendFuncs(1, spl::BlendFunc::One, spl::BlendFunc::One, spl::BlendFunc::One, spl::BlendFunc::One);

			spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_transparencyFramebuffer);
			spl::Framebuffer::clear(true, false, false);

			std::multimap<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceTransparency*>, const Drawable*> transparencyDrawSequence;
			for (const Drawable* drawable : _drawables)
			{
				_insertInDrawSequence(&transparencyDrawSequence, drawable, ShaderType::Transparency);
			}

			const spl::ShaderProgram* currentShader = nullptr;
			for (const std::pair<std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceTransparency*>, const Drawable*>& elt : transparencyDrawSequence)
			{
				if (elt.first.first != currentShader)
				{
					currentShader = elt.first.first;
					spl::ShaderProgram::bind(currentShader);

					_setGBufferResultUniforms(elt.first.first, &elt.first.second->u_gBuffer);
					_setShadowResultUniforms(elt.first.first, &elt.first.second->u_shadow);
				}

				context->setFaceCullingMode(elt.second->getGeometryFaceCullingMode());
				context->setFaceCullingOrientation(elt.second->getGeometryFaceCullingOrientation());

				_updateAndBindUboDrawable(2, _camera, elt.second);

				_setDrawableUniforms(elt.first.first, &elt.first.second->u_drawable, elt.second);

				elt.second->_draw();
			}

			context->setIsBlendEnabled(0, false);
			context->setIsBlendEnabled(1, false);
		}
		
		// Compute SSAO

		context->setFaceCullingMode(spl::FaceCullingMode::Disabled);
		context->setIsDepthTestEnabled(false);
		
		if (_params.ssaoEnabled)
		{
			spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_ssaoFramebuffer);

			const spl::ShaderProgram* ssaoShader = _shaders[0];

			spl::ShaderProgram::bind(ssaoShader);

			static constexpr shaderInterface::SubInterfaceGBufferResult ssaoGBufferResultInterface = {
				.depth = spl::GlslType::Sampler2d,
				.normal = spl::GlslType::Sampler2d,
				.tangent = spl::GlslType::Sampler2d
			};
			_setGBufferResultUniforms(ssaoShader, &ssaoGBufferResultInterface);

			static constexpr shaderInterface::SubInterfaceShadowResult ssaoShadowResultInterface = {};
			_setShadowResultUniforms(ssaoShader, &ssaoShadowResultInterface);

			static constexpr shaderInterface::SubInterfaceTransparencyResult ssaoTransparencyResultInterface = {};
			_setTransparencyResultUniforms(ssaoShader, &ssaoTransparencyResultInterface);

			ssaoShader->setUniform("u_sampleCount", _params.ssaoSampleCount);

			ssaoShader->setUniform("u_scaleStep", 1.f / (_params.ssaoSampleCount * _params.ssaoSampleCount));
			ssaoShader->setUniform("u_tanHalfFov", std::tan(_camera->getFieldOfView() * 0.5f));
			ssaoShader->setUniform("u_twoTanHalfFov", 2.f * std::tan(_camera->getFieldOfView() * 0.5f));
			ssaoShader->setUniform("u_resolution", _resolution);

			_screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		}
		
		// Merge into final picture

		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_mergeFramebuffer);

		const uint32_t mergeShaderIndex = _background ? (_background->getCreationParams().target == spl::TextureTarget::CubeMap ? 3 : 2) : 1;
		const spl::ShaderProgram* mergeShader = _shaders[mergeShaderIndex];
		
		spl::ShaderProgram::bind(mergeShader);

		static constexpr shaderInterface::SubInterfaceGBufferResult mergeGBufferResultInterface = {
			.depth = spl::GlslType::Sampler2d,
			.color = spl::GlslType::Sampler2d,
			.material = spl::GlslType::Sampler2d,
			.normal = spl::GlslType::Sampler2d
		};
		_setGBufferResultUniforms(mergeShader, &mergeGBufferResultInterface);
		
		if (_params.shadowEnabled)
		{
			static constexpr shaderInterface::SubInterfaceShadowResult mergeShadowResultInterface = {
				.texture = spl::GlslType::Sampler2dArrayShadow,
				.offset = spl::GlslType::FloatVec3
			};
			_setShadowResultUniforms(mergeShader, &mergeShadowResultInterface);
		}
		
		if (_params.transparencyEnabled)
		{
			static constexpr shaderInterface::SubInterfaceTransparencyResult mergeTransparencyResultInterface = {
				.color = spl::GlslType::Sampler2d,
				.counter = spl::GlslType::Sampler2d
			};
			_setTransparencyResultUniforms(mergeShader, &mergeTransparencyResultInterface);
		}

		if (_params.ssaoEnabled)
		{
			static constexpr shaderInterface::SubInterfaceSsaoResult mergeSsaoResultInterface = {
				.texture = spl::GlslType::Sampler2d
			};
			_setSsaoResultUniforms(mergeShader, &mergeSsaoResultInterface);
		}
		
		if (_background)
		{
			mergeShader->setUniform("u_background", 12, _background);
		}
		else
		{
			mergeShader->setUniform("u_background", _clearColor);
		}
		
		mergeShader->setUniform("u_twoTanHalfFov", 2.f * std::tan(_camera->getFieldOfView() * 0.5f));
		
		_screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		
		// Restore OpenGL context

		context->setState(savedContextState);
	}

	const spl::Texture2D& Scene::getDepthTexture() const
	{
		return _depthTexture;
	}

	const spl::Texture2D& Scene::getColorTexture() const
	{
		return _colorTexture;
	}

	const spl::Texture2D& Scene::getMaterialTexture() const
	{
		return _materialTexture;
	}

	const spl::Texture2D& Scene::getNormalTexture() const
	{
		return _normalTexture;
	}

	const spl::Texture2D& Scene::getTangentTexture() const
	{
		return _tangentTexture;
	}

	const spl::Texture& Scene::getShadowTexture() const
	{
		return _shadowTexture;
	}

	const spl::Texture2D& Scene::getTransparencyColorTexture() const
	{
		return _transparencyColorTexture;
	}

	const spl::Texture2D& Scene::getTransparencyCounterTexture() const
	{
		return _transparencyCounterTexture;
	}

	const spl::Texture2D& Scene::getSsaoTexture() const
	{
		return _ssaoTexture;
	}

	const spl::Texture2D& Scene::getRenderTexture() const
	{
		return _mergeTexture;
	}

	Scene::~Scene()
	{
		for (spl::ShaderProgram* shader : _shaders)
		{
			delete shader;
		}
	}

	void Scene::_loadShaders()
	{
		std::string headerString = std::format(
			"#version 460 core\n"
			"#define LIGHT_SLOT_COUNT {}\n"
			"#define SHADOW_TEXTURE_COUNT {}\n",

			_params.lightSlotCount,
			_params.shadowTextureCount
		);

		if (_params.shadowEnabled)
		{
			headerString += "#define SHADOW\n";
		}

		if (_params.transparencyEnabled)
		{
			headerString += "#define TRANSPARENCY\n";
		}

		if (_params.ssaoEnabled)
		{
			headerString += "#define SSAO\n";
		}

		const std::string_view header =					{ headerString.data(), headerString.size() };
		const std::string_view background =				"#define BACKGROUND_TEXTURE\n";
		const std::string_view backgroundProjection =	"#define BACKGROUND_PROJECTION\n";
		const std::string_view backgroundCubemap =		"#define BACKGROUND_CUBEMAP\n";
		const std::string_view colorMap =				"#define COLOR_TEXTURE\n";
		const std::string_view materialMap =			"#define MATERIAL_TEXTURE\n";
		const std::string_view normalMap =				"#define NORMAL_TEXTURE\n";


		const std::pair<spl::ShaderStage::Stage, std::vector<std::string_view>> sources[] = {

			// Drawable independent

			/* 00 */{ spl::ShaderStage::Vertex,		{ header, common_glsl, 															ssao_vert } },
			/* 01 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,  														ssao_frag } },
			/* 02 */{ spl::ShaderStage::Vertex,		{ header, common_glsl,  														merge_vert } },
			/* 03 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl,   									merge_frag, lighting_glsl } },
			/* 04 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, background,	backgroundProjection,	merge_frag, lighting_glsl } },
			/* 05 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, background,	backgroundCubemap,		merge_frag, lighting_glsl } },

			// Drawable dependent - Mesh

			/* 06 */{ spl::ShaderStage::Vertex,		{ header, common_glsl,						 										mesh_gBuffer_vert } },
			/* 07 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						 										mesh_gBuffer_frag } },
			/* 08 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						colorMap,								mesh_gBuffer_frag } },
			/* 09 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						 			materialMap,				mesh_gBuffer_frag } },
			/* 10 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						colorMap,	materialMap,				mesh_gBuffer_frag } },
			/* 11 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						 							normalMap,	mesh_gBuffer_frag } },
			/* 12 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						colorMap,					normalMap,	mesh_gBuffer_frag } },
			/* 13 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						 			materialMap,	normalMap,	mesh_gBuffer_frag } },
			/* 14 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						colorMap,	materialMap,	normalMap,	mesh_gBuffer_frag } },
			/* 15 */{ spl::ShaderStage::Vertex,		{ header, common_glsl,						 										mesh_shadowMapping_vert } },
			/* 16 */{ spl::ShaderStage::Geometry,	{ header, common_glsl,						 										mesh_shadowMapping_geom } },
			/* 17 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,																mesh_shadowMapping_frag } },
			/* 18 */{ spl::ShaderStage::Fragment,	{ header, common_glsl,						colorMap,								mesh_shadowMapping_frag } },
			/* 19 */{ spl::ShaderStage::Vertex,		{ header, common_glsl,  															mesh_transparency_vert } },
			/* 20 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl,  										mesh_transparency_frag, lighting_glsl } },
			/* 21 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, colorMap,								mesh_transparency_frag, lighting_glsl } },
			/* 22 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl,  			materialMap,				mesh_transparency_frag, lighting_glsl } },
			/* 23 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, colorMap,	materialMap,				mesh_transparency_frag, lighting_glsl } },
			/* 24 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl,  							normalMap,	mesh_transparency_frag, lighting_glsl } },
			/* 25 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, colorMap,					normalMap,	mesh_transparency_frag, lighting_glsl } },
			/* 26 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl,  			materialMap,	normalMap,	mesh_transparency_frag, lighting_glsl } },
			/* 27 */{ spl::ShaderStage::Fragment,	{ header, common_glsl, lightingHeader_glsl, colorMap,	materialMap,	normalMap,	mesh_transparency_frag, lighting_glsl } },
		};

		static constexpr uint32_t count = sizeof(sources) / sizeof(sources[0]);

		spl::ShaderModule modules[count];
		for (uint32_t i = 0; i < count; ++i)
		{
			std::vector<const char*> srcPtrs;
			std::vector<uint32_t> srcSizes;
			for (const std::string_view& src : sources[i].second)
			{
				srcPtrs.push_back(src.data());
				srcSizes.push_back(src.size());
			}

			modules[i].createFromGlsl(sources[i].first, srcPtrs.data(), srcSizes.data(), sources[i].second.size());
		}

		std::array<const spl::ShaderModule*, 5> moduleArray;

		for (spl::ShaderProgram* shader : _shaders)
		{
			delete shader;
		}
		_shaders.clear();

		// Drawable independent

		moduleArray = { modules + 0, modules + 1, nullptr, nullptr, nullptr };			// 00
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 2, modules + 3, nullptr, nullptr, nullptr };			// 01
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 2, modules + 4, nullptr, nullptr, nullptr };			// 02
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 2, modules + 5, nullptr, nullptr, nullptr };			// 03
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));

		// Drawable dependent - Mesh

		moduleArray = { modules + 6, modules + 7, nullptr, nullptr, nullptr };			// 04
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 8, nullptr, nullptr, nullptr };			// 05
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 9, nullptr, nullptr, nullptr };			// 06
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 10, nullptr, nullptr, nullptr };			// 07
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 11, nullptr, nullptr, nullptr };			// 08
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 12, nullptr, nullptr, nullptr };			// 09
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 13, nullptr, nullptr, nullptr };			// 10
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 14, nullptr, nullptr, nullptr };			// 11
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 15, modules + 16, modules + 17, nullptr, nullptr };	// 12
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 3));
		moduleArray = { modules + 15, modules + 16, modules + 18, nullptr, nullptr };	// 13
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 3));
		moduleArray = { modules + 19, modules + 20, nullptr, nullptr, nullptr };		// 14
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 21, nullptr, nullptr, nullptr };		// 15
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 22, nullptr, nullptr, nullptr };		// 16
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 23, nullptr, nullptr, nullptr };		// 17
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 24, nullptr, nullptr, nullptr };		// 18
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 25, nullptr, nullptr, nullptr };		// 19
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 26, nullptr, nullptr, nullptr };		// 20
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 19, modules + 27, nullptr, nullptr, nullptr };		// 21
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));


		_shaderMap = {
			{ 
				DrawableType::Particles,
				{
					// Nothing
				}
			},
			{
				DrawableType::Mesh,
				{
					{
						{ _shaders[4],	_shaders[12], _shaders[14] },	//
						{ _shaders[5],	_shaders[13], _shaders[15] },	// colorMap
						{ _shaders[6],	_shaders[12], _shaders[16] },	// 			  materialMap
						{ _shaders[7],	_shaders[13], _shaders[17] },	// colorMap + materialMap
						{ _shaders[8],	_shaders[12], _shaders[18] },	// 							normalMap
						{ _shaders[9],	_shaders[13], _shaders[19] },	// colorMap					normalMap
						{ _shaders[10],	_shaders[12], _shaders[20] },	// 			  materialMap + normalMap
						{ _shaders[11],	_shaders[13], _shaders[21] }	// colorMap + materialMap + normalMap
					}
				}
			},
			{
				DrawableType::MeshInstanced,
				{
					// etc...
				}
			},
			{
				DrawableType::Group,
				{
					// Nothing
				}
			}
		};
	}

	const void Scene::_updateAndBindUboLights(uint32_t index, std::vector<const CameraBase*>& shadowCameras)
	{
		assert(_lights.size() < _params.lightSlotCount);

		shadowCameras.clear();

		// Might be too big for the stack
		thread_local static UboLightsData uboLightsData;

		uint32_t i = 0;
		for (const LightBase* light : _lights)
		{
			uboLightsData.lights[i].type = static_cast<uint32_t>(light->getType());

			uboLightsData.lights[i].shadowMapStartIndex = shadowCameras.size();
			if (_params.shadowEnabled && light->getShadowCascadeSize() != 0)
			{
				light->_getShadowCameras(_camera, shadowCameras);
			}
			uboLightsData.lights[i].shadowMapStopIndex = shadowCameras.size();

			uboLightsData.lights[i].color = light->getColor() * light->getIntensity();

			light->_getUboParams(_camera, &uboLightsData.lights[i].param0);

			++i;
		}
		uboLightsData.count = i;

		_uboLights.update(&uboLightsData, offsetof(UboLightsData, lights) + sizeof(UboLightData) * i);

		spl::Buffer::bind(spl::BufferTarget::Uniform, &_uboLights, index);
	}

	const void Scene::_updateAndBindUboDrawable(uint32_t index, const CameraBase* camera, const Drawable* drawable)
	{
		UboDrawableData uboDrawableData;

		uboDrawableData.model = drawable->getTransformMatrix();
		uboDrawableData.invModel = drawable->getInverseTransformMatrix();
		uboDrawableData.viewModel = camera->getViewMatrix() * uboDrawableData.model;
		uboDrawableData.invViewModel = uboDrawableData.invModel * camera->getInverseViewMatrix();
		uboDrawableData.projectionViewModel = camera->getProjectionMatrix() * uboDrawableData.viewModel;
		uboDrawableData.invProjectionViewModel = uboDrawableData.invViewModel * camera->getInverseProjectionMatrix();

		_uboDrawable.update(&uboDrawableData, sizeof(UboDrawableData));

		spl::Buffer::bind(spl::BufferTarget::Uniform, &_uboDrawable, index);
	}

	const void Scene::_updateAndBindUboShadowCameras(uint32_t index, const std::vector<const CameraBase*>& shadowCameras)
	{
		assert(shadowCameras.size() <= _params.shadowTextureCount);

		if (_params.shadowEnabled)
		{
			// Might be too big for the stack
			thread_local static UboShadowCamerasData uboShadowCamerasData;

			for (uint32_t i = 0; i < shadowCameras.size(); ++i)
			{
				uboShadowCamerasData.cameras[i].projection = shadowCameras[i]->getProjectionMatrix();
				uboShadowCamerasData.cameras[i].view = shadowCameras[i]->getViewMatrix();
				uboShadowCamerasData.cameras[i].near = shadowCameras[i]->getNearDistance();
				uboShadowCamerasData.cameras[i].far = shadowCameras[i]->getFarDistance();
			}
			uboShadowCamerasData.count = shadowCameras.size();

			_uboShadowCameras.update(&uboShadowCamerasData, offsetof(UboShadowCamerasData, cameras) + sizeof(UboShadowCameraData) * shadowCameras.size());
		}

		spl::Buffer::bind(spl::BufferTarget::Uniform, &_uboShadowCameras, index);
	}

	void Scene::_insertInDrawSequence(void* pDrawSequence, const Drawable* drawable, ShaderType shaderType) const
	{
		DrawableType drawableType = drawable->getType();

		if (drawableType == DrawableType::Group)
		{
			const DrawableGroup* group = dynamic_cast<const DrawableGroup*>(drawable);
			assert(group != nullptr);

			uint32_t childCount = group->_getChildCount();
			for (uint32_t i = 0; i < childCount; ++i)
			{
				_insertInDrawSequence(pDrawSequence, group->_getChild(i), shaderType);
			}
		}
		else
		{
			const ShaderSet* drawableShaderSet = drawable->getShaderSet();
			const Material* material = drawable->getMaterial();

			uint32_t index = (material->getColorTexture() != nullptr) | ((material->getPropertiesTexture() != nullptr) << 1);
			if (drawableType == DrawableType::Mesh)
			{
				index |= (dynamic_cast<const Mesh<>*>(drawable)->getNormalMap() != nullptr) << 2;
			}

			switch (shaderType)
			{
				case ShaderType::GBuffer:
				{
					using TKey = std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceGBuffer*>;

					TKey key;
					if (drawableShaderSet)
					{
						key.first = drawableShaderSet->_shaderGBuffer;
						key.second = &drawableShaderSet->_interfaceGBuffer;
					}
					else
					{
						key.first = _shaderMap.find(drawableType)->second[index]._shaderGBuffer;
						key.second = &_shaderMap.find(drawableType)->second[index]._interfaceGBuffer;
					}

					std::multimap<TKey, const Drawable*>& drawSequence = *reinterpret_cast<std::multimap<TKey, const Drawable*>*>(pDrawSequence);
					drawSequence.insert({ key, drawable });

					break;
				}
				case ShaderType::ShadowMapping:
				{
					using TKey = std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceShadow*>;

					TKey key;
					if (drawableShaderSet)
					{
						key.first = drawableShaderSet->_shaderShadow;
						key.second = &drawableShaderSet->_interfaceShadow;
					}
					else
					{
						key.first = _shaderMap.find(drawableType)->second[index]._shaderShadow;
						key.second = &_shaderMap.find(drawableType)->second[index]._interfaceShadow;
					}

					std::multimap<TKey, const Drawable*>& drawSequence = *reinterpret_cast<std::multimap<TKey, const Drawable*>*>(pDrawSequence);
					drawSequence.insert({ key, drawable });

					break;
				}
				case ShaderType::Transparency:
				{
					using TKey = std::pair<const spl::ShaderProgram*, const shaderInterface::InterfaceTransparency*>;

					TKey key;
					if (drawableShaderSet)
					{
						key.first = drawableShaderSet->_shaderTransparency;
						key.second = &drawableShaderSet->_interfaceTransparency;
					}
					else
					{
						key.first = _shaderMap.find(drawableType)->second[index]._shaderTransparency;
						key.second = &_shaderMap.find(drawableType)->second[index]._interfaceTransparency;
					}

					std::multimap<TKey, const Drawable*>& drawSequence = *reinterpret_cast<std::multimap<TKey, const Drawable*>*>(pDrawSequence);
					drawSequence.insert({ key, drawable });

					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}
	
	void Scene::_setDrawableUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceDrawable* interface, const Drawable* drawable)
	{
		const Material* material = drawable->getMaterial();

		if (interface->color == spl::GlslType::Sampler2d)
		{
			assert(material->getColorTexture());
			shader->setUniform("u_drawableColor", 0, material->getColorTexture());
		}
		else if (interface->color == spl::GlslType::FloatVec4)
		{
			shader->setUniform("u_drawableColor", material->getColor());
		}

		if (interface->material == spl::GlslType::Sampler2d)
		{
			assert(material->getPropertiesTexture());
			shader->setUniform("u_drawableMaterial", 1, material->getPropertiesTexture());
		}
		else if (interface->material == spl::GlslType::FloatVec3)
		{
			shader->setUniform("u_drawableMaterial", material->getProperties());
		}

		switch (drawable->getType())
		{
			case DrawableType::Mesh:
			{
				const Mesh<>& mesh = *dynamic_cast<const Mesh<>*>(drawable);

				if (interface->normal == spl::GlslType::Sampler2d)
				{
					assert(mesh.getNormalMap());
					shader->setUniform("u_drawableNormal", 2, mesh.getNormalMap());
				}

				break;
			}
			default:
			{
				break;
			}
		}
	}

	void Scene::_setGBufferResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceGBufferResult* interface)
	{
		if (interface->depth == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_gBufferDepth", 3, &getDepthTexture());
		}

		if (interface->color == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_gBufferColor", 4, &getColorTexture());
		}

		if (interface->material == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_gBufferMaterial", 5, &getMaterialTexture());
		}

		if (interface->normal == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_gBufferNormal", 6, &getNormalTexture());
		}

		if (interface->tangent == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_gBufferTangent", 7, &getTangentTexture());
		}
	}

	void Scene::_setShadowResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceShadowResult* interface)
	{
		if (interface->texture == spl::GlslType::Sampler2dArrayShadow)
		{
			shader->setUniform("u_shadowTexture", 8, &getShadowTexture());
		}

		if (interface->offset == spl::GlslType::FloatVec3)
		{
			shader->setUniform("u_shadowOffset", scp::f32vec3(1.f / _params.shadowTextureResolution.x, 1.f / _params.shadowTextureResolution.y, 1e-4f));
		}
	}

	void Scene::_setTransparencyResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceTransparencyResult* interface)
	{
		if (interface->color == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_transparencyColor", 9, &getTransparencyColorTexture());
		}

		if (interface->counter == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_transparencyCounter", 10, &getTransparencyCounterTexture());
		}
	}

	void Scene::_setSsaoResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceSsaoResult* interface)
	{
		if (interface->texture == spl::GlslType::Sampler2d)
		{
			shader->setUniform("u_ssaoTexture", 11, &getSsaoTexture());
		}
	}
}
