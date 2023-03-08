///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

#include "Shaders/shaders.cpp"

namespace lys
{
	namespace
	{
		#pragma pack(push, 1)
		struct alignas(16) UboShadowCameraData
		{
			alignas(16) scp::f32mat4x4 projection;
			alignas(16) scp::f32mat4x4 view;
			alignas(4) float near;
			alignas(4) float far;
		};

		struct alignas(16) UboShadowCamerasData
		{
			alignas(16) UboShadowCameraData cameras[Scene::maxShadowMapCount];
		};

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

			alignas(16) UboLightData lights[Scene::maxLightCount];
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
		#pragma pack(pop)
	}

	Scene::Scene(uint32_t width, uint32_t height) :
		_shaders(),
		_shaderMap(),

		_depthTexture(),
		_colorTexture(),
		_materialTexture(),
		_normalTexture(),
		_tangentTexture(),
		_gBufferFramebuffer(),

		_shadowTextures(),
		_shadowMappingFramebuffer(),

		_ssaoTexture(),
		_ssaoFramebuffer(),

		_mergeTexture(),
		_mergeFramebuffer(),

		_uboShadowCameras(sizeof(UboShadowCamerasData), spl::BufferStorageFlags::DynamicStorage),

		_clearColor(0.f, 0.f, 0.f),
		_background(nullptr),

		_resolution(width, height),
		_camera(nullptr),

		_lights(),
		_uboLights(sizeof(UboLightsData), spl::BufferStorageFlags::DynamicStorage),

		_drawables(),
		_uboDrawable(sizeof(UboDrawableData), spl::BufferStorageFlags::DynamicStorage),

		_screenVao(),
		_screenVbo()
	{
		_loadShaders();


		spl::TextureCreationParams shadowTextureCreationParams;
		shadowTextureCreationParams.target = spl::TextureTarget::Array2D;
		shadowTextureCreationParams.internalFormat = spl::TextureInternalFormat::Depth_nu16;
		shadowTextureCreationParams.width = _shadowMapResolution.x;
		shadowTextureCreationParams.height = _shadowMapResolution.y;
		shadowTextureCreationParams.depth = maxShadowMapCount;
		_shadowTextures.createNew(shadowTextureCreationParams);
		_shadowTextures.setBorderColor(1.f, 1.f, 1.f, 1.f);
		_shadowTextures.setWrappingS(spl::TextureWrapping::ClampToBorder);
		_shadowTextures.setWrappingT(spl::TextureWrapping::ClampToBorder);

		resize(width, height);


		_screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
		_screenVao.setAttributeEnabled(0, true);
		_screenVao.setAttributeBinding(0, 0);
		_screenVao.setBindingDivisor(0, 0);

		static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
		_screenVbo.createNew(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);
		_screenVao.bindArrayBuffer(&_screenVbo, 0, sizeof(float) * 2);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		_resolution.x = width;
		_resolution.y = height;

		_depthTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::Depth_nu32);
		_colorTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::RGB_nu16);
		_materialTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::RGB_nu16);
		_normalTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::RGB_ni16);
		_tangentTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::RGB_ni16);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::DepthAttachment, 0, &_depthTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_colorTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 1, &_materialTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 2, &_normalTexture);
		_gBufferFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 3, &_tangentTexture);

		_ssaoTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::R_nu16);
		_ssaoFramebuffer.attachTexture(spl::FramebufferAttachment::ColorAttachment, 0, &_ssaoTexture);

		_mergeTexture.createNew(_resolution.x, _resolution.y, spl::TextureInternalFormat::RGB_nu16);
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
		assert(isValid());

		// Save context context - `render` must not appear to modify context state from caller pov

		spl::Context* context = spl::Context::getCurrentContext();
		spl::ContextState savedContextState = context->getState();

		// Set context state for rendering
		
		context->setClearColor(0.f, 0.f, 0.f, 0.f);
		context->setClearDepth(1.0);
		context->setClearStencil(0);
		context->setViewport(0, 0, _resolution.x, _resolution.y);
		context->setIsSeamlessCubeMapFilteringEnabled(true);
		context->setIsDepthTestEnabled(true);
		context->setFaceCulling(spl::FaceCulling::FrontClockWise);
		
		// Update and bind static UBOs
		
		std::vector<const CameraBase*> shadowCameras;
		
		// 0 :	CameraData
		_updateAndBindUboLights(1, shadowCameras);
		// 2 :	DrawableData
		_updateAndBindUboShadowCameras(3, shadowCameras);
		
		// Draw G-Buffer
		
		_camera->updateAndBindUbo(0);
		
		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_gBufferFramebuffer);
		spl::Framebuffer::clear(true, true, false);
		
		std::multimap<std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>, const Drawable*> gBufferDrawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(&gBufferDrawSequence, drawable, ShaderType::GBuffer);
		}
		
		const spl::ShaderProgram* currentShader = nullptr;
		for (const std::pair<std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>, const Drawable*>& elt : gBufferDrawSequence)
		{
			if (elt.first.first != currentShader)
			{
				currentShader = elt.first.first;
				spl::ShaderProgram::bind(currentShader);
			}
		
			_updateAndBindUboDrawable(2, _camera, elt.second);
			_setGBufferUniforms(elt.first, elt.second);
			elt.second->_draw();
		}
		
		// Draw shadow maps
		
		context->setViewport(0, 0, _shadowMapResolution.x, _shadowMapResolution.y);
		context->setFaceCulling(spl::FaceCulling::BackClockWise);
		
		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_shadowMappingFramebuffer);
		spl::Framebuffer::bind(spl::FramebufferTarget::ReadFramebuffer, &_shadowMappingFramebuffer);
		
		std::multimap<std::pair<const spl::ShaderProgram*, const ShadowMappingShaderInterface*>, const Drawable*> shadowMappingDrawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(&shadowMappingDrawSequence, drawable, ShaderType::ShadowMapping);
		}
		
		spl::TextureUpdateParams shadowMapUpdateParams;
		shadowMapUpdateParams.framebufferData = &_shadowMappingFramebuffer;
		shadowMapUpdateParams.width = _shadowMapResolution.x;
		shadowMapUpdateParams.height = _shadowMapResolution.y;

		uint32_t shadowIndex = 0;
		for (const CameraBase* shadowCamera : shadowCameras)
		{
			spl::Framebuffer::clear(false, true, false);
		
			shadowCamera->updateAndBindUbo(0);
		
			const spl::ShaderProgram* currentShader = nullptr;
			for (const std::pair<std::pair<const spl::ShaderProgram*, const ShadowMappingShaderInterface*>, const Drawable*>& elt : shadowMappingDrawSequence)
			{
				if (elt.first.first != currentShader)
				{
					currentShader = elt.first.first;
					spl::ShaderProgram::bind(currentShader);
				}
		
				_updateAndBindUboDrawable(2, shadowCamera, elt.second);
				_setShadowMappingUniforms(elt.first, elt.second);
				elt.second->_draw();
			}

			_shadowMappingFramebuffer.attachTexture(spl::FramebufferAttachment::DepthAttachment, 0, &_shadowTextures, 0, shadowIndex);
			++shadowIndex;
		}
		
		context->setViewport(0, 0, _resolution.x, _resolution.y);
		context->setFaceCulling(spl::FaceCulling::FrontClockWise);
		_camera->updateAndBindUbo(0);
		
		// Compute SSAO
		
		context->setIsDepthTestEnabled(false);
		
		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_ssaoFramebuffer);
		spl::Framebuffer::clear(true, false, false);
		
		const spl::ShaderProgram* ssaoShader = _shaders[3];
		
		spl::ShaderProgram::bind(ssaoShader);
		
		ssaoShader->setUniform("u_depth", 0, getDepthTexture());
		ssaoShader->setUniform("u_normal", 1, getNormalTexture());
		ssaoShader->setUniform("u_tangent", 2, getTangentTexture());
		
		ssaoShader->setUniform("u_sampleCount", uint32_t(16));
		ssaoShader->setUniform("u_radius", 1.f);
		
		ssaoShader->setUniform("u_scaleStep", 1.f / (16 * 16));
		ssaoShader->setUniform("u_tanHalfFov", std::tan(_camera->getFieldOfView() * 0.5f));
		
		_screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		
		// Merge into final picture
		
		context->setClearColor(_clearColor.x, _clearColor.y, _clearColor.z, 1.f);
		
		spl::Framebuffer::bind(spl::FramebufferTarget::DrawFramebuffer, &_mergeFramebuffer);
		spl::Framebuffer::clear(true, false, false);
		
		const uint32_t mergeShaderIndex = _background ? (_background->getCreationParams().target == spl::TextureTarget::CubeMap ? 2 : 1) : 0;
		const spl::ShaderProgram* mergeShader = _shaders[mergeShaderIndex];
		
		spl::ShaderProgram::bind(mergeShader);
		
		mergeShader->setUniform("u_depth", 0, getDepthTexture());
		mergeShader->setUniform("u_color", 1, getColorTexture());
		mergeShader->setUniform("u_material", 2, getMaterialTexture());
		mergeShader->setUniform("u_normal", 3, getNormalTexture());
		
		mergeShader->setUniform("u_shadow", 4, getShadowTextures());
		
		mergeShader->setUniform("u_ssao", 5, getSsaoTexture());
		
		if (_background)
		{
			mergeShader->setUniform("u_background", 6, _background);
		}
		
		mergeShader->setUniform("u_tanHalfFov", std::tan(_camera->getFieldOfView() * 0.5f));
		mergeShader->setUniform("u_blurOffset", scp::f32vec2(1.f / _resolution.x, 1.f / _resolution.y));
		
		_screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		
		// Restore OpenGL context

		context->setState(savedContextState);
	}

	const spl::Texture2D* Scene::getDepthTexture() const
	{
		return &_depthTexture;
	}

	const spl::Texture2D* Scene::getColorTexture() const
	{
		return &_colorTexture;
	}

	const spl::Texture2D* Scene::getMaterialTexture() const
	{
		return &_materialTexture;
	}

	const spl::Texture2D* Scene::getNormalTexture() const
	{
		return &_normalTexture;
	}

	const spl::Texture2D* Scene::getTangentTexture() const
	{
		return &_tangentTexture;
	}

	const spl::Texture* Scene::getShadowTextures() const
	{
		return &_shadowTextures;
	}

	const spl::Texture2D* Scene::getSsaoTexture() const
	{
		return &_ssaoTexture;
	}

	const spl::Texture2D* Scene::getRenderTexture() const
	{
		return &_mergeTexture;
	}

	bool Scene::isValid() const
	{
		return _camera != nullptr;
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
		static const std::string headerString = std::format(
			"#version 460 core\n"
			"#define MAX_LIGHT_COUNT {}\n"
			"#define MAX_SHADOWMAP_COUNT {}\n",

			Scene::maxLightCount,
			Scene::maxShadowMapCount
		);

		static const std::string_view header =					{ headerString.data(), headerString.size() };
		static const std::string_view background =				"#define BACKGROUND\n";
		static const std::string_view backgroundProjection =	"#define BACKGROUND_PROJECTION\n";
		static const std::string_view backgroundCubemap =		"#define BACKGROUND_CUBEMAP\n";
		static const std::string_view colorMap =				"#define COLOR_MAP\n";
		static const std::string_view materialMap =				"#define MATERIAL_MAP\n";
		static const std::string_view normalMap =				"#define NORMAL_MAP\n";


		static const std::pair<spl::ShaderStage::Stage, std::vector<std::string_view>> sources[] = {
			{ spl::ShaderStage::Vertex,		{ header, 													common_glsl, merge_vert } },
			{ spl::ShaderStage::Fragment,	{ header,  													common_glsl, merge_frag } },
			{ spl::ShaderStage::Fragment,	{ header, background,	backgroundProjection,				common_glsl, merge_frag } },
			{ spl::ShaderStage::Fragment,	{ header, background,	backgroundCubemap,					common_glsl, merge_frag } },
			{ spl::ShaderStage::Vertex,		{ header,													common_glsl, ssao_vert } },
			{ spl::ShaderStage::Fragment,	{ header,													common_glsl, ssao_frag } },
			{ spl::ShaderStage::Vertex,		{ header, 													common_glsl, mesh_shadowMapping_vert } },
			{ spl::ShaderStage::Fragment,	{ header, 													common_glsl, mesh_shadowMapping_frag } },
			{ spl::ShaderStage::Vertex,		{ header, 													common_glsl, mesh_gBuffer_vert } },
			{ spl::ShaderStage::Fragment,	{ header, 													common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,											common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 				materialMap,						common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,		materialMap,						common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 										normalMap,	common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,								normalMap,	common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 				materialMap,			normalMap,	common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,		materialMap,			normalMap,	common_glsl, mesh_gBuffer_frag } }
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

		moduleArray = { modules + 0, modules + 1, nullptr, nullptr, nullptr };	// 0
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 0, modules + 2, nullptr, nullptr, nullptr };	// 1
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 0, modules + 3, nullptr, nullptr, nullptr };	// 2
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 5, nullptr, nullptr, nullptr };	// 3
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 6, modules + 7, nullptr, nullptr, nullptr };	// 4
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 9, nullptr, nullptr, nullptr };	// 5
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 10, nullptr, nullptr, nullptr };	// 6
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 11, nullptr, nullptr, nullptr };	// 7
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 12, nullptr, nullptr, nullptr };	// 8
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 13, nullptr, nullptr, nullptr };	// 9
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 14, nullptr, nullptr, nullptr };	// 10
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 15, nullptr, nullptr, nullptr };	// 11
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 8, modules + 16, nullptr, nullptr, nullptr };	// 12
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
						{ _shaders[5], _shaders[4] },	//
						{ _shaders[6], _shaders[4] },	// colorMap
						{ _shaders[7], _shaders[4] },	// 			  materialMap
						{ _shaders[8], _shaders[4] },	// colorMap + materialMap
						{ _shaders[9], _shaders[4] },	// 							normalMap
						{ _shaders[10], _shaders[4] },	// colorMap					normalMap
						{ _shaders[11], _shaders[4] },	// 			  materialMap + normalMap
						{ _shaders[12], _shaders[4] }	// colorMap + materialMap + normalMap
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
		thread_local static UboLightsData uboLightsData;

		uint32_t i = 0;
		for (const LightBase* light : _lights)
		{
			uboLightsData.lights[i].type = static_cast<uint32_t>(light->getType());

			uboLightsData.lights[i].shadowMapStartIndex = shadowCameras.size();
			if (light->getCastShadows())
			{
				light->_getShadowCameras(_camera, shadowCameras);
			}
			uboLightsData.lights[i].shadowMapStopIndex = shadowCameras.size();

			uboLightsData.lights[i].color = light->getColor() * light->getIntensity();

			light->_getUboParams(_camera, &uboLightsData.lights[i].param0);

			++i;
		}
		uboLightsData.count = i;

		_uboLights.update(&uboLightsData, 4 * sizeof(float) + sizeof(UboLightData) * i);

		spl::Buffer::bind(spl::BufferTarget::Uniform, &_uboLights, index);
	}

	const void Scene::_updateAndBindUboDrawable(uint32_t index, const CameraBase* camera, const Drawable* drawable)
	{
		thread_local static UboDrawableData uboDrawableData;

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
		assert(shadowCameras.size() <= maxShadowMapCount);

		thread_local static UboShadowCamerasData uboShadowCamerasData;

		for (uint32_t i = 0; i < shadowCameras.size(); ++i)
		{
			uboShadowCamerasData.cameras[i].projection = shadowCameras[i]->getProjectionMatrix();
			uboShadowCamerasData.cameras[i].view = shadowCameras[i]->getViewMatrix();
			uboShadowCamerasData.cameras[i].near = shadowCameras[i]->getNearDistance();
			uboShadowCamerasData.cameras[i].far = shadowCameras[i]->getFarDistance();
		}

		_uboShadowCameras.update(&uboShadowCamerasData, sizeof(UboShadowCameraData) * shadowCameras.size());

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

			switch (shaderType)
			{
				case ShaderType::GBuffer:
				{
					using TKey = std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>;

					TKey key;
					if (drawableShaderSet)
					{
						key.first = drawableShaderSet->_gBufferShader;
						key.second = &drawableShaderSet->_gBufferShaderInterface;
					}
					else
					{
						uint32_t index = (material->getColorTexture() != nullptr) | ((material->getPropertiesTexture() != nullptr) << 1);
						if (drawableType == DrawableType::Mesh)
						{
							index |= (dynamic_cast<const Mesh<>*>(drawable)->getNormalMap() != nullptr) << 2;
						}

						key.first = _shaderMap.find(drawableType)->second[index]._gBufferShader;
						key.second = &_shaderMap.find(drawableType)->second[index]._gBufferShaderInterface;
					}

					std::multimap<TKey, const Drawable*>& drawSequence = *reinterpret_cast<std::multimap<TKey, const Drawable*>*>(pDrawSequence);
					drawSequence.insert({ key, drawable });

					break;
				}
				case ShaderType::ShadowMapping:
				{
					using TKey = std::pair<const spl::ShaderProgram*, const ShadowMappingShaderInterface*>;

					TKey key;
					if (drawableShaderSet)
					{
						key.first = drawableShaderSet->_shadowMappingShader;
						key.second = &drawableShaderSet->_shadowMappingShaderInterface;
					}
					else
					{
						key.first = _shaderMap.find(drawableType)->second[4]._shadowMappingShader;
						key.second = &_shaderMap.find(drawableType)->second[4]._shadowMappingShaderInterface;
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
	
	void Scene::_setGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const Drawable* drawable)
	{
		switch (drawable->getType())
		{
			case DrawableType::Mesh:
			{
				const Mesh<>& mesh = *dynamic_cast<const Mesh<>*>(drawable);

				if (gBuffer.second->u_normalMap == spl::GlslType::Sampler2d)
				{
					assert(mesh.getNormalMap());
					gBuffer.first->setUniform("u_normalMap", 2, mesh.getNormalMap());
				}

				break;
			}
			default:
			{
				break;
			}
		}


		const Material* material = drawable->getMaterial();

		if (gBuffer.second->u_color == spl::GlslType::Sampler2d)
		{
			assert(material->getColorTexture());
			gBuffer.first->setUniform("u_color", 0, material->getColorTexture());
		}
		else if (gBuffer.second->u_color == spl::GlslType::FloatVec4)
		{
			gBuffer.first->setUniform("u_color", material->getColor());
		}

		if (gBuffer.second->u_material == spl::GlslType::Sampler2d)
		{
			assert(material->getPropertiesTexture());
			gBuffer.first->setUniform("u_material", 1, material->getPropertiesTexture());
		}
		else if (gBuffer.second->u_material == spl::GlslType::FloatVec3)
		{
			gBuffer.first->setUniform("u_material", material->getProperties());
		}
	}
	
	void Scene::_setShadowMappingUniforms(const std::pair<const spl::ShaderProgram*, const ShadowMappingShaderInterface*>& gBuffer, const Drawable* drawable)
	{

	}
}
