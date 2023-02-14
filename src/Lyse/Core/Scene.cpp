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
	thread_local std::unordered_map<DrawableType, std::vector<DrawableShaderSet>> Scene::_shaderMap;

	Scene::Scene(uint32_t width, uint32_t height) :
		_gBufferFramebuffer(),
		_mergeFramebuffer(),
		_camera(nullptr),
		_drawables()
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

	void Scene::addDrawable(const Drawable* drawable)
	{
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
		}

		void ContextSave::restore() const
		{
			spl::Context* context = spl::Context::getCurrentContext();

			context->setClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
			context->setClearDepth(clearDepth);
			context->setClearStencil(clearStencil);

			spl::Framebuffer::bind(*framebuffer, spl::FramebufferTarget::DrawFramebuffer);
			spl::ShaderProgram::bind(*shader);
		}
		
		struct DrawableShaderSetLess
		{
			bool operator()(const DrawableShaderSet& a, const DrawableShaderSet& b) const
			{
				static std::hash<const spl::ShaderProgram*> h;

				const size_t x = h(a.gBufferShader) * 3 + 0;
				const size_t y = h(b.gBufferShader) * 3 + 0;

				return x < y;
			}
		};
	}

	void Scene::render() const
	{
		assert(isValid());

		// Save OpenGL context - `render` must not appear to modify context from caller pov
		
		ContextSave contextSave;
		contextSave.save();

		// Set context

		spl::Context* context = spl::Context::getCurrentContext();
		context->setClearColor(0.f, 0.f, 0.f, 0.f);
		context->setClearDepth(1.f);
		context->setClearStencil(0);
		
		// Compute draw sequence - The less shader binding, the better
		
		std::multimap<DrawableShaderSet, const Drawable*, DrawableShaderSetLess> drawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(&drawSequence, drawable);
		}
		
		// Draw gBuffer
		
		spl::Framebuffer::bind(_gBufferFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear(true, true, false);
		
		DrawContext drawContext;
		drawContext.shader = nullptr;
		drawContext.transform = 1.f;
		
		for (const std::pair<DrawableShaderSet, const Drawable*>& elt : drawSequence)
		{
			if (elt.first.gBufferShader != drawContext.shader)
			{
				drawContext.shader = elt.first.gBufferShader;
		
				spl::ShaderProgram::bind(*drawContext.shader);

				drawContext.shader->setUniform("u_projection", _camera->getProjectionMatrix());
				drawContext.shader->setUniform("u_view", _camera->getViewMatrix());
			}
		
			const Material& material = elt.second->_getMaterial();

			drawContext.shader->setUniform("u_color", material._color);
			drawContext.shader->setUniform("u_material", material._props);

			elt.second->_draw(drawContext);
		}

		// Merge into final picture
		
		spl::Framebuffer::bind(_mergeFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear();

		static spl::VertexArray screenVao;
		static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
		static spl::Buffer screenVbo;

		if (!screenVbo.isValid())
		{
			screenVbo.createNew(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);

			screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
			screenVao.setAttributeEnabled(0, true);
			screenVao.setAttributeBinding(0, 0);
			screenVao.setBindingDivisor(0, 0);

			screenVao.bindArrayBuffer(screenVbo, 0, 0, sizeof(float) * 2);
		}

		spl::ShaderProgram::bind(*_shaders[0]);
		_shaders[0]->setUniform("u_depth", 0, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::DepthAttachment));
		_shaders[0]->setUniform("u_color", 1, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
		_shaders[0]->setUniform("u_material", 2, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment1));
		_shaders[0]->setUniform("u_position", 3, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment2));
		_shaders[0]->setUniform("u_normal", 4, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment3));
		// _shaders[0]->setUniform("u_tangent", 5, *_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment4));

		_shaders[0]->setUniform("u_cameraPos", _camera->getTranslation());

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

	void Scene::_insertInDrawSequence(void* pDrawSequence, const Drawable* drawable)
	{
		DrawableType drawableType = drawable->_getType();

		if (drawableType == DrawableType::Group)
		{
			const DrawableGroup* group = dynamic_cast<const DrawableGroup*>(drawable);
			assert(group != nullptr);

			uint32_t childCount = group->_getChildCount();
			for (uint32_t i = 0; i < childCount; ++i)
			{
				_insertInDrawSequence(pDrawSequence, group->_getChild(i));
			}
		}
		else
		{
			std::multimap<DrawableShaderSet, const Drawable*, DrawableShaderSetLess>& drawSequence = *reinterpret_cast<std::multimap<DrawableShaderSet, const Drawable*, DrawableShaderSetLess>*>(pDrawSequence);

			DrawableShaderSet set = drawable->_getShaderSet();

			if (!set.gBufferShader /*|| !set.mergeShader*/)
			{
				// TODO: Compute index from material and drawable

				if (!set.gBufferShader)
				{
					set.gBufferShader = _shaderMap[drawableType][0].gBufferShader;
				}
			}

			drawSequence.insert({ set, drawable });
		}
	}

	void Scene::_loadShaders()
	{
		spl::ShaderModule modules[] = {
			{ spl::ShaderStage::Vertex,		merge_vert,			sizeof(merge_vert) },
			{ spl::ShaderStage::Fragment,	merge_frag,			sizeof(merge_frag) },
			{ spl::ShaderStage::Vertex,		mesh_gBuffer_vert,	sizeof(mesh_gBuffer_vert) },
			{ spl::ShaderStage::Fragment,	mesh_gBuffer_frag,	sizeof(mesh_gBuffer_frag) }
		};

		std::array<const spl::ShaderModule*, 5> moduleArray;

		// Merge
		moduleArray = { modules + 0, modules + 1, nullptr, nullptr, nullptr };
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		// Mesh gBuffer
		moduleArray = { modules + 2, modules + 3, nullptr, nullptr, nullptr };
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
					{ _shaders[1] }
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
}
