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
	thread_local uint32_t Scene::_sceneCount(0);
	thread_local std::vector<spl::ShaderProgram*> Scene::_shaders;
	thread_local std::unordered_map<DrawableType, std::vector<ShaderSet>> Scene::_shaderMap;

	namespace
	{
		#pragma pack(push, 1)
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
			scp::f32vec3 padding;
			UboLightData lights[Scene::maxLightCount];
		};
		#pragma pack(pop)
	}

	Scene::Scene(uint32_t width, uint32_t height) :
		_gBufferFramebuffer(),
		_mergeFramebuffer(),
		_camera(nullptr),
		_clearColor(0.f, 0.f, 0.f),
		_background(nullptr),
		_drawables(),
		_lights(),
		_uboLights(sizeof(UboLightsData), spl::BufferUsage::StreamDraw)
	{
		if (++_sceneCount == 1)
		{
			_loadShaders();
		}

		resize(width, height);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		/* Depth    */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::DepthAttachment, scp::u32vec2{ width, height }, spl::TextureInternalFormat::Depth_nu32);
		// TODO : Stencil
		/* Color    */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
		/* Material */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment1, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
		/* Position */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment2, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_f32);
		/* Normal   */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment3, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_ni16);
		/* Tangent  */ _gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment4, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_ni16);
		
		_mergeFramebuffer.createNewRenderbufferAttachment(spl::FramebufferAttachment::DepthStencilAttachment, spl::TextureInternalFormat::Depth_nu24_Stencil_u8, width, height);
		_mergeFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
	}

	void Scene::setCamera(const CameraBase* camera)
	{
		_camera = camera;
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
				_setCameraGBufferUniforms(elt.first, _camera);
			}

			_setDrawableGBufferUniforms(elt.first, elt.second);
			elt.second->_draw();
		}

		// Merge into final picture

		context->setClearColor(_clearColor.x, _clearColor.y, _clearColor.z, 1.f);
		spl::Framebuffer::bind(_mergeFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear();

		static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
		thread_local static spl::VertexArray screenVao;
		thread_local static spl::Buffer screenVbo;

		if (!screenVbo.isValid())
		{
			screenVbo.createNew(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);

			screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
			screenVao.setAttributeEnabled(0, true);
			screenVao.setAttributeBinding(0, 0);
			screenVao.setBindingDivisor(0, 0);

			screenVao.bindArrayBuffer(screenVbo, 0, 0, sizeof(float) * 2);
		}
		
		const spl::ShaderProgram* mergeShader = _shaders[0];
		if (_background)
		{
			if (_background->getCreationParams().target == spl::TextureTarget::CubeMap)
			{
				mergeShader = _shaders[2];
			}
			else
			{
				mergeShader = _shaders[1];
			}
		}

		spl::ShaderProgram::bind(*mergeShader);
		mergeShader->setUniform("u_depth", 0, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::DepthAttachment));
		mergeShader->setUniform("u_color", 1, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
		mergeShader->setUniform("u_material", 2, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment1));
		mergeShader->setUniform("u_position", 3, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment2));
		mergeShader->setUniform("u_normal", 4, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment3));
		// mergeShader->setUniform("u_tangent", 5, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment4));

		mergeShader->setUniform("u_cameraPos", _camera->getTranslation());

		if (_background)
		{
			mergeShader->setUniform("u_cameraUp", _camera->getUpVector());
			mergeShader->setUniform("u_cameraFront", _camera->getFrontVector());
			mergeShader->setUniform("u_cameraLeft", _camera->getLeftVector());
			mergeShader->setUniform("u_ndcToCamera", scp::f32vec2(_camera->getAspect(), 1.f) * std::tan(_camera->getFieldOfView() / 2.f));
			mergeShader->setUniform("u_background", 5, *_background);
		}

		thread_local static UboLightsData lightsData;

		uint32_t i = 0;
		for (const LightBase* light : _lights)
		{
			lightsData.lights[i].type = static_cast<uint32_t>(light->getType());
			lightsData.lights[i].color = light->getColor() * light->getIntensity();
			light->_getParams(&lightsData.lights[i].param0);
			++i;
		}
		lightsData.count = i;

		_uboLights.update(&lightsData, 4 * sizeof(float) + sizeof(UboLightData) * i);

		spl::Buffer::bind(_uboLights, spl::BufferTarget::Uniform, 0);
		mergeShader->setUniformBlockBinding(0, 0);

		screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);
		
		// Restore OpenGL context
		
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

	const spl::Texture2D& Scene::getPositionTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment2));
	}

	const spl::Texture2D& Scene::getNormalTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment3));
	}

	const spl::Texture2D& Scene::getTangentTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment4));
	}

	const spl::Texture2D& Scene::getRenderTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_mergeFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	bool Scene::isValid() const
	{
		return _camera != nullptr;
	}

	void Scene::_loadShaders()
	{
		constexpr char header[] =				"#version 460 core\n";
		constexpr char background[] =			"#define BACKGROUND\n";
		constexpr char backgroundProjection[] =	"#define BACKGROUND_PROJECTION\n";
		constexpr char backgroundCubemap[] =	"#define BACKGROUND_CUBEMAP\n";
		constexpr char colorMap[] =				"#define COLOR_MAP\n";
		constexpr char materialMap[] =			"#define MATERIAL_MAP\n";
		constexpr char normalMap[] =			"#define NORMAL_MAP\n";

		std::string lightCount = "#define MAX_LIGHT_COUNT " + std::to_string(Scene::maxLightCount) + "\n";

		const std::vector<const char*> sources[] = {
			{ header, 																merge_vert },
			{ header, lightCount.data(), 											merge_frag },
			{ header, lightCount.data(),	background,		backgroundProjection,	merge_frag },
			{ header, lightCount.data(),	background,		backgroundCubemap,		merge_frag },
			{ header, 																mesh_gBuffer_vert },
			{ header, 																mesh_gBuffer_frag },
			{ header, colorMap,														mesh_gBuffer_frag },
			{ header, 						materialMap,							mesh_gBuffer_frag },
			{ header, colorMap,				materialMap,							mesh_gBuffer_frag },
			{ header, 										normalMap,				mesh_gBuffer_frag },
			{ header, colorMap,								normalMap,				mesh_gBuffer_frag },
			{ header, 						materialMap,	normalMap,				mesh_gBuffer_frag },
			{ header, colorMap,				materialMap,	normalMap,				mesh_gBuffer_frag }
		};

		const std::vector<uint32_t> sizes[] = {
			{ sizeof(header) - 1,																						sizeof(merge_vert) },
			{ sizeof(header) - 1, lightCount.size(),																	sizeof(merge_frag) },
			{ sizeof(header) - 1, lightCount.size(),	sizeof(background) - 1,		sizeof(backgroundProjection) - 1,	sizeof(merge_frag) },
			{ sizeof(header) - 1, lightCount.size(),	sizeof(background) - 1,		sizeof(backgroundCubemap) - 1,		sizeof(merge_frag) },
			{ sizeof(header) - 1,																						sizeof(mesh_gBuffer_vert) },
			{ sizeof(header) - 1,																						sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1, sizeof(colorMap) - 1,																	sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1,						sizeof(materialMap) - 1,										sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1, sizeof(colorMap) - 1,	sizeof(materialMap) - 1,										sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1,													sizeof(normalMap) - 1,				sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1, sizeof(colorMap) - 1,								sizeof(normalMap) - 1,				sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1,						sizeof(materialMap) - 1,	sizeof(normalMap) - 1,				sizeof(mesh_gBuffer_frag) },
			{ sizeof(header) - 1, sizeof(colorMap) - 1,	sizeof(materialMap) - 1,	sizeof(normalMap) - 1,				sizeof(mesh_gBuffer_frag) }
		};

		spl::ShaderModule modules[] = {
			{ spl::ShaderStage::Vertex,		sources[0].data(),	sizes[0].data(),	uint32_t(sources[0].size()) },
			{ spl::ShaderStage::Fragment,	sources[1].data(),	sizes[1].data(),	uint32_t(sources[1].size()) },
			{ spl::ShaderStage::Fragment,	sources[2].data(),	sizes[2].data(),	uint32_t(sources[2].size()) },
			{ spl::ShaderStage::Fragment,	sources[3].data(),	sizes[3].data(),	uint32_t(sources[3].size()) },
			{ spl::ShaderStage::Vertex,		sources[4].data(),	sizes[4].data(),	uint32_t(sources[4].size()) },
			{ spl::ShaderStage::Fragment,	sources[5].data(),	sizes[5].data(),	uint32_t(sources[5].size()) },
			{ spl::ShaderStage::Fragment,	sources[6].data(),	sizes[6].data(),	uint32_t(sources[6].size()) },
			{ spl::ShaderStage::Fragment,	sources[7].data(),	sizes[7].data(),	uint32_t(sources[7].size()) },
			{ spl::ShaderStage::Fragment,	sources[8].data(),	sizes[8].data(),	uint32_t(sources[8].size()) },
			{ spl::ShaderStage::Fragment,	sources[9].data(),	sizes[9].data(),	uint32_t(sources[9].size()) },
			{ spl::ShaderStage::Fragment,	sources[10].data(),	sizes[10].data(),	uint32_t(sources[10].size()) },
			{ spl::ShaderStage::Fragment,	sources[11].data(),	sizes[11].data(),	uint32_t(sources[11].size()) },
			{ spl::ShaderStage::Fragment,	sources[12].data(),	sizes[12].data(),	uint32_t(sources[12].size()) }
		};

		std::array<const spl::ShaderModule*, 5> moduleArray;

		moduleArray = { modules + 0, modules + 1, nullptr, nullptr, nullptr };	// 0
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 0, modules + 2, nullptr, nullptr, nullptr };	// 1
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 0, modules + 3, nullptr, nullptr, nullptr };	// 2
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 5, nullptr, nullptr, nullptr };	// 3
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 6, nullptr, nullptr, nullptr };	// 4
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 7, nullptr, nullptr, nullptr };	// 5
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 8, nullptr, nullptr, nullptr };	// 6
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 9, nullptr, nullptr, nullptr };	// 7
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 10, nullptr, nullptr, nullptr };	// 8
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 11, nullptr, nullptr, nullptr };	// 9
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		moduleArray = { modules + 4, modules + 12, nullptr, nullptr, nullptr };	// 10
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
						_shaders[3],	//
						_shaders[4],	// colorMap
						_shaders[5],	// 			  materialMap
						_shaders[6],	// colorMap + materialMap
						_shaders[7],	// 							normalMap
						_shaders[8],	// colorMap					normalMap
						_shaders[9],	// 			  materialMap + normalMap
						_shaders[10]	// colorMap + materialMap + normalMap
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

	void Scene::_insertInDrawSequence(void* pDrawSequence, const Drawable* drawable, ShaderType shaderType)
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

						key.first = _shaderMap[drawableType][index]._gBufferShader;
						key.second = &_shaderMap[drawableType][index]._gBufferShaderInterface;
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
	
	void Scene::_setCameraGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const CameraBase* camera)
	{
		if (gBuffer.second->u_projection == spl::GlslType::FloatMat4x4)
		{
			gBuffer.first->setUniform("u_projection", camera->getProjectionMatrix());
		}

		if (gBuffer.second->u_view == spl::GlslType::FloatMat4x4)
		{
			gBuffer.first->setUniform("u_view", camera->getViewMatrix());
		}
	}

	void Scene::_setDrawableGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const Drawable* drawable)
	{
		if (gBuffer.second->u_model == spl::GlslType::FloatMat4x4)
		{
			gBuffer.first->setUniform("u_model", drawable->getTransformMatrix());
		}

		const Material* material = drawable->getMaterial();

		if (gBuffer.second->u_color == spl::GlslType::Sampler2d)
		{
			assert(material->getColorTexture());
			gBuffer.first->setUniform("u_color", 0, *material->getColorTexture());
		}
		else
		{
			gBuffer.first->setUniform("u_color", material->getColor());
		}

		if (gBuffer.second->u_material == spl::GlslType::Sampler2d)
		{
			assert(material->getPropertiesTexture());
			gBuffer.first->setUniform("u_material", 1, *material->getPropertiesTexture());
		}
		else
		{
			gBuffer.first->setUniform("u_material", material->getProperties());
		}

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

	}
}
