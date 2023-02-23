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
		struct UboCameraData
		{
			float aspect;
			float fov;
			float near;
			float far;

			scp::f32vec3 up;
				uint32_t _padding0[1];
			scp::f32vec3 front;
				uint32_t _padding1[1];
			scp::f32vec3 left;
				uint32_t _padding2[1];

			scp::f32mat4x4 view;
			scp::f32mat4x4 invView;
			scp::f32mat4x4 projection;
			scp::f32mat4x4 invProjection;
			scp::f32mat4x4 projectionView;
			scp::f32mat4x4 invProjectionView;
		};

		struct UboLightData
		{
			scp::f32vec3 color;
			uint32_t type;

			scp::f32vec4 param0;
			scp::f32vec4 param1;
		};

		struct UboLightsData
		{
			uint32_t count;
				scp::f32vec3 _padding;

			UboLightData lights[Scene::maxLightCount];
		};

		struct UboDrawableData
		{
			scp::f32mat4x4 model;
			scp::f32mat4x4 invModel;
			scp::f32mat4x4 viewModel;
			scp::f32mat4x4 invViewModel;
			scp::f32mat4x4 projectionViewModel;
			scp::f32mat4x4 invProjectionViewModel;
		};
		#pragma pack(pop)
	}

	Scene::Scene(uint32_t width, uint32_t height) :
		_shaders(),
		_shaderMap(),
		_gBufferFramebuffer(),
		_ssaoFramebuffer(),
		_mergeFramebuffer(),
		_clearColor(0.f, 0.f, 0.f),
		_background(nullptr),
		_resolution(width, height),
		_camera(nullptr),
		_uboCamera(sizeof(UboCameraData), spl::BufferUsage::StreamDraw),
		_lights(),
		_uboLights(sizeof(UboLightsData), spl::BufferUsage::StreamDraw),
		_drawables(),
		_uboDrawable(sizeof(UboDrawableData), spl::BufferUsage::StreamDraw),
		_screenVao(),
		_screenVbo()
	{
		_loadShaders();
		resize(width, height);

		_shadowMappingFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::DepthAttachment, scp::u32vec2{ 4096, 4096 }, spl::TextureInternalFormat::Depth_nu32);

		_screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
		_screenVao.setAttributeEnabled(0, true);
		_screenVao.setAttributeBinding(0, 0);
		_screenVao.setBindingDivisor(0, 0);

		static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
		_screenVbo.createNew(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);
		_screenVao.bindArrayBuffer(_screenVbo, 0, 0, sizeof(float) * 2);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		_resolution.x = width;
		_resolution.y = height;

		/* Depth    */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::DepthAttachment, scp::u32vec2{ width, height }, spl::TextureInternalFormat::Depth_nu32);
		// TODO : Stencil
		/* Color    */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
		/* Material */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment1, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
		/* Normal   */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment2, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_ni16);
		/* Tangent  */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment3, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_ni16);

		_ssaoFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::R_nu16);

		_mergeFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
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
		_background = &texture->getRawTexture();
	}

	void Scene::setBackgroundCubemap(const spl::RawTexture* texture)
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

	namespace
	{
		struct ContextSave
		{
			scp::f32vec4 clearColor;
			double clearDepth;
			int32_t clearStencil;

			const spl::Framebuffer* framebuffer;
			const spl::ShaderProgram* shader;

			const spl::Buffer* uniformBlockBuffer;

			void save();
			void restore() const;
		};

		void ContextSave::save()
		{
			spl::Context* context = spl::Context::getCurrentContext();

			clearColor = context->getClearColor();
			clearDepth = context->getClearDepth();
			clearStencil = context->getClearStencil();

			framebuffer = context->getFramebufferBinding(spl::FramebufferTarget::DrawFramebuffer);
			shader = context->getShaderBinding();

			uniformBlockBuffer = context->getBufferBinding(spl::BufferTarget::Uniform, 0);
		}

		void ContextSave::restore() const
		{
			spl::Context* context = spl::Context::getCurrentContext();

			context->setClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			context->setClearDepth(clearDepth);
			context->setClearStencil(clearStencil);
			
			if (framebuffer)			spl::Framebuffer::bind(*framebuffer, spl::FramebufferTarget::DrawFramebuffer);
			if (shader)					spl::ShaderProgram::bind(*shader);
			if (uniformBlockBuffer)		spl::Buffer::bind(*uniformBlockBuffer, spl::BufferTarget::Uniform, 0);
		}
	}

	void Scene::render() const
	{
		assert(isValid());

		// Save OpenGL context - `render` must not appear to modify context from caller pov
		
		ContextSave contextSave;
		contextSave.save();

		// Set context

		spl::Context* context = spl::Context::getCurrentContext();
		context->setClearDepth(1.f);
		context->setClearStencil(0);

		// Update UBOs

		const UboCameraData* uboCameraData = reinterpret_cast<const UboCameraData*>(_updateUboCamera());
		const UboLightsData* uboLightsData = reinterpret_cast<const UboLightsData*>(_updateUboLights());
		
		// Draw G-Buffer

		context->setClearColor(0.f, 0.f, 0.f, 0.f);
		spl::Framebuffer::bind(_gBufferFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
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

				spl::ShaderProgram::bind(*currentShader);
			}

			_updateUboDrawable(uboCameraData, elt.second);

			_setGBufferUniforms(elt.first, elt.second);

			elt.second->_draw();
		}

		// Draw shadow maps
		
		const LightBase* light = *_lights.begin();
		
		scp::f32vec4 p[8] = {
			{-1.f, -1.f, -1.f, 1.f},
			{ 1.f, -1.f, -1.f, 1.f},
			{-1.f,  1.f, -1.f, 1.f},
			{ 1.f,  1.f, -1.f, 1.f},
			{-1.f, -1.f,  1.f, 1.f},
			{ 1.f, -1.f,  1.f, 1.f},
			{-1.f,  1.f,  1.f, 1.f},
			{ 1.f,  1.f,  1.f, 1.f}
		};
		
		for (uint8_t i = 0; i < 8; ++i)
		{
			p[i] = _camera->getInverseViewMatrix() * _camera->getInverseProjectionMatrix() * p[i];
			p[i] /= p[i].w;
			light->applyInverseRotationTo(p[i].xyz());
		}
		
		std::array<float, 6> bbox = { p[0].x, p[0].x, p[0].y, p[0].y, p[0].z, p[0].z };
		for (uint8_t i = 1; i < 8; ++i)
		{
			bbox[0] = std::min(bbox[0], p[i].x);
			bbox[1] = std::max(bbox[1], p[i].x);
			bbox[2] = std::min(bbox[2], p[i].y);
			bbox[3] = std::max(bbox[3], p[i].y);
			bbox[4] = std::min(bbox[4], p[i].z);
			bbox[5] = std::max(bbox[5], p[i].z);
		}
		
		scp::f32vec3 pos = {
			(bbox[0] + bbox[1]) / 2.f,
			(bbox[2] + bbox[3]) / 2.f,
			bbox[5] + 10.f	// TODO : Better than that magic number
		};
		
		scp::f32vec3 size = {
			bbox[1] - bbox[0],
			bbox[3] - bbox[2],
			pos.z - bbox[4]
		};
		
		light->applyRotationTo(pos);
		
		scp::f32vec3 direction = { 0.f, 0.f, -1.f };
		light->applyRotationTo(direction);
		
		CameraOrthographic lightCamera(size.x, size.y, size.z);
		lightCamera.setPosition(pos);
		lightCamera.setDirection(direction);
		
		context->setViewport(0, 0, 4096, 4096);
		
		spl::Framebuffer::bind(_shadowMappingFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear(false, true, false);
		
		const spl::ShaderProgram* shadowMappingShader = _shaders[4];
		
		spl::ShaderProgram::bind(*shadowMappingShader);
		
		shadowMappingShader->setUniform("u_projection", lightCamera.getProjectionMatrix());
		shadowMappingShader->setUniform("u_near", lightCamera.getNearDistance());
		shadowMappingShader->setUniform("u_far", lightCamera.getFarDistance());
		
		for (const Drawable* drawable : _drawables)
		{
			shadowMappingShader->setUniform("u_viewModel", lightCamera.getViewMatrix() * drawable->getTransformMatrix());
			drawable->_draw();
		}
		
		context->setViewport(0, 0, _resolution.x, _resolution.y);

		// Compute SSAO

		context->setIsDepthTestEnabled(false);
		spl::Framebuffer::bind(_ssaoFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear(true, false, false);

		const spl::ShaderProgram* ssaoShader = _shaders[3];

		spl::ShaderProgram::bind(*ssaoShader);

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
		spl::Framebuffer::bind(_mergeFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear(true, false, false);
		
		const uint32_t mergeShaderIndex = _background ? (_background->getCreationParams().target == spl::TextureTarget::CubeMap ? 2 : 1) : 0;
		const spl::ShaderProgram* mergeShader = _shaders[mergeShaderIndex];

		spl::ShaderProgram::bind(*mergeShader);

		mergeShader->setUniform("u_depth", 0, getDepthTexture());
		mergeShader->setUniform("u_color", 1, getColorTexture());
		mergeShader->setUniform("u_material", 2, getMaterialTexture());
		mergeShader->setUniform("u_normal", 3, getNormalTexture());

		mergeShader->setUniform("u_shadow", 4, getShadowMap());
		mergeShader->setUniform("u_lightProj", lightCamera.getProjectionMatrix());
		mergeShader->setUniform("u_lightView", lightCamera.getViewMatrix());
		mergeShader->setUniform("u_lightNear", lightCamera.getNearDistance());
		mergeShader->setUniform("u_lightFar", lightCamera.getFarDistance());

		mergeShader->setUniform("u_ssao", 5, getSsaoTexture());

		if (_background)
		{
			mergeShader->setUniform("u_background", 6, *_background);
		}

		mergeShader->setUniform("u_tanHalfFov", std::tan(_camera->getFieldOfView() * 0.5f));
		mergeShader->setUniform("u_blurOffset", scp::f32vec2(2.f / _resolution.x, 2.f / _resolution.y));
		
		_screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		
		// Restore OpenGL context

		context->setIsDepthTestEnabled(true);	// TODO : include that and face culling in context save/restore
		contextSave.restore();
	}

	const spl::Texture2D& Scene::getDepthTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::DepthAttachment));
	}

	const spl::Texture2D& Scene::getStencilTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::StencilAttachment));
	}

	const spl::Texture2D& Scene::getColorTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	const spl::Texture2D& Scene::getMaterialTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment1));
	}

	const spl::Texture2D& Scene::getNormalTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment2));
	}

	const spl::Texture2D& Scene::getTangentTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment3));
	}

	const spl::Texture2D& Scene::getShadowMap() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_shadowMappingFramebuffer.getTextureAttachment(spl::FramebufferAttachment::DepthAttachment));
	}

	const spl::Texture2D& Scene::getSsaoTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_ssaoFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	const spl::Texture2D& Scene::getRenderTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_mergeFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
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
		static const std::string lightCountData =				"#define MAX_LIGHT_COUNT " + std::to_string(Scene::maxLightCount) + "\n";

		static const std::string_view header =					"#version 460 core\n";
		static const std::string_view background =				"#define BACKGROUND\n";
		static const std::string_view backgroundProjection =	"#define BACKGROUND_PROJECTION\n";
		static const std::string_view backgroundCubemap =		"#define BACKGROUND_CUBEMAP\n";
		static const std::string_view colorMap =				"#define COLOR_MAP\n";
		static const std::string_view materialMap =				"#define MATERIAL_MAP\n";
		static const std::string_view normalMap =				"#define NORMAL_MAP\n";
		static const std::string_view lightCount =				{ lightCountData.data(), lightCountData.size() };


		static const std::pair<spl::ShaderStage::Stage, std::vector<std::string_view>> sources[] = {
			{ spl::ShaderStage::Vertex,		{ header, 														common_glsl, merge_vert } },
			{ spl::ShaderStage::Fragment,	{ header, lightCount, 											common_glsl, merge_frag } },
			{ spl::ShaderStage::Fragment,	{ header, lightCount,		background,	backgroundProjection,	common_glsl, merge_frag } },
			{ spl::ShaderStage::Fragment,	{ header, lightCount,		background,	backgroundCubemap,		common_glsl, merge_frag } },
			{ spl::ShaderStage::Vertex,		{ header,														common_glsl, ssao_vert } },
			{ spl::ShaderStage::Fragment,	{ header,														common_glsl, ssao_frag } },
			{ spl::ShaderStage::Vertex,		{ header, 														common_glsl, mesh_shadowMapping_vert } },
			{ spl::ShaderStage::Fragment,	{ header, 														common_glsl, mesh_shadowMapping_frag } },
			{ spl::ShaderStage::Vertex,		{ header, 														common_glsl, mesh_gBuffer_vert } },
			{ spl::ShaderStage::Fragment,	{ header, 														common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,												common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 						materialMap,					common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,				materialMap,					common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 									normalMap,			common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,							normalMap,			common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, 						materialMap,	normalMap,		common_glsl, mesh_gBuffer_frag } },
			{ spl::ShaderStage::Fragment,	{ header, colorMap,				materialMap,	normalMap,		common_glsl, mesh_gBuffer_frag } }
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
						_shaders[5],	//
						_shaders[6],	// colorMap
						_shaders[7],	// 			  materialMap
						_shaders[8],	// colorMap + materialMap
						_shaders[9],	// 							normalMap
						_shaders[10],	// colorMap					normalMap
						_shaders[11],	// 			  materialMap + normalMap
						_shaders[12]	// colorMap + materialMap + normalMap
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

	const void* Scene::_updateUboCamera() const
	{
		thread_local static UboCameraData uboCameraData;

		uboCameraData.aspect = _camera->getAspect();
		uboCameraData.fov = _camera->getFieldOfView();
		uboCameraData.near = _camera->getNearDistance();
		uboCameraData.far = _camera->getFarDistance();

		uboCameraData.up = _camera->getUpVector();
		uboCameraData.front = _camera->getFrontVector();
		uboCameraData.left = _camera->getLeftVector();

		uboCameraData.view = _camera->getViewMatrix();
		uboCameraData.invView = _camera->getInverseViewMatrix();
		uboCameraData.projection = _camera->getProjectionMatrix();
		uboCameraData.invProjection = _camera->getInverseProjectionMatrix();
		uboCameraData.projectionView = uboCameraData.projection * uboCameraData.view;
		uboCameraData.invProjectionView = uboCameraData.invView * uboCameraData.invProjection;

		_uboCamera.update(&uboCameraData, sizeof(UboCameraData));

		spl::Buffer::bind(_uboCamera, spl::BufferTarget::Uniform, 0);

		return &uboCameraData;
	}

	const void* Scene::_updateUboLights() const
	{
		thread_local static UboLightsData uboLightsData;

		uint32_t i = 0;
		for (const LightBase* light : _lights)
		{
			uboLightsData.lights[i].color = light->getColor() * light->getIntensity();
			uboLightsData.lights[i].type = static_cast<uint32_t>(light->getType());
			light->_getParams(_camera->getViewMatrix(), &uboLightsData.lights[i].param0);
			++i;
		}
		uboLightsData.count = i;

		_uboLights.update(&uboLightsData, 4 * sizeof(float) + sizeof(UboLightData) * i);

		spl::Buffer::bind(_uboLights, spl::BufferTarget::Uniform, 1);

		return &uboLightsData;
	}

	const void* Scene::_updateUboDrawable(const void* pUboCameraData, const Drawable* drawable) const
	{
		thread_local static UboDrawableData uboDrawableData;
		const UboCameraData* uboCameraData = reinterpret_cast<const UboCameraData*>(pUboCameraData);

		uboDrawableData.model = drawable->getTransformMatrix();
		uboDrawableData.invModel = drawable->getInverseTransformMatrix();
		uboDrawableData.viewModel = uboCameraData->view * uboDrawableData.model;
		uboDrawableData.invViewModel = uboDrawableData.invModel * uboCameraData->invView;
		uboDrawableData.projectionViewModel = uboCameraData->projection * uboDrawableData.viewModel;
		uboDrawableData.invProjectionViewModel = uboDrawableData.invViewModel * uboCameraData->invProjection;

		_uboDrawable.update(&uboDrawableData, sizeof(UboDrawableData));

		spl::Buffer::bind(_uboDrawable, spl::BufferTarget::Uniform, 2);

		return &uboDrawableData;
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
					gBuffer.first->setUniform("u_normalMap", 2, *mesh.getNormalMap());
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
			gBuffer.first->setUniform("u_color", 0, *material->getColorTexture());
		}
		else if (gBuffer.second->u_color == spl::GlslType::FloatVec4)
		{
			gBuffer.first->setUniform("u_color", material->getColor());
		}

		if (gBuffer.second->u_material == spl::GlslType::Sampler2d)
		{
			assert(material->getPropertiesTexture());
			gBuffer.first->setUniform("u_material", 1, *material->getPropertiesTexture());
		}
		else if (gBuffer.second->u_material == spl::GlslType::FloatVec3)
		{
			gBuffer.first->setUniform("u_material", material->getProperties());
		}
	}
}
