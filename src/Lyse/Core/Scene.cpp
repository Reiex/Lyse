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
	thread_local std::unordered_map<DrawableType, std::unordered_map<uint64_t, DrawableShaderSet>> Scene::_shaderMap;

	Scene::Scene(uint32_t width, uint32_t height) :
		_gBufferFramebuffer(),
		_camera(nullptr),
		_drawables()
	{
		if (++_sceneCount == 1)
		{
			_loadShaders();
		}

		/* Depth    */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::DepthAttachment, scp::u32vec2{ width, height }, spl::TextureInternalFormat::Depth_nu32);
		// TODO : /* Stencil  */_gBufferFramebuffer.createNewRenderbufferAttachment(spl::FramebufferAttachment::StencilAttachment, spl::TextureInternalFormat::Stencil_u1, width, height);
		/* Position */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_f32);
		/* Normal   */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment1, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_f32);
		/* Albedo   */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment2, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		/* Depth    */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::DepthAttachment, scp::u32vec2{ width, height }, spl::TextureInternalFormat::Depth_nu32);
		// TODO : /* Stencil  */_gBufferFramebuffer.createNewRenderbufferAttachment(spl::FramebufferAttachment::StencilAttachment, spl::TextureInternalFormat::Stencil_u1, width, height);
		/* Position */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_f32);
		/* Normal   */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment1, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_f32);
		/* Albedo   */_gBufferFramebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment2, scp::u32vec2{ width, height }, spl::TextureInternalFormat::RGB_nu8);
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

				const size_t x = h(a.gBufferShader) * 3 + h(a.mergeShader);
				const size_t y = h(b.gBufferShader) * 3 + h(b.mergeShader);

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
		
		spl::Framebuffer::bind(_gBufferFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear(true, true, false);
		
		// Compute draw sequence - The less shader binding, the better
		
		std::multimap<DrawableShaderSet, const Drawable*, DrawableShaderSetLess> drawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(&drawSequence, drawable);
		}
		
		// Draw elements
		
		DrawContext drawContext;
		drawContext.program = nullptr;
		drawContext.transform = 1.f;
		
		for (const std::pair<DrawableShaderSet, const Drawable*>& elt : drawSequence)
		{
			if (elt.first.gBufferShader != drawContext.program)
			{
				drawContext.program = elt.first.gBufferShader;
		
				spl::ShaderProgram::bind(*drawContext.program);

				drawContext.program->setUniform("u_projection", _camera->getProjectionMatrix());
				drawContext.program->setUniform("u_view", _camera->getViewMatrix());
			}
		
			elt.second->_draw(drawContext);
		}
		
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

	const spl::Texture2D& Scene::getPositionTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	const spl::Texture2D& Scene::getNormalTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment1));
	}

	const spl::Texture2D& Scene::getAlbedoTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_gBufferFramebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment2));
	}

	bool Scene::isValid() const
	{
		return _camera != nullptr;
	}

	void Scene::_insertInDrawSequence(void* pDrawSequence, const Drawable* drawable)
	{
		const DrawableInfo& info = drawable->_getInfo();

		if (info.type == DrawableType::Group)
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

			DrawableShaderSet set = info.shaderSet;

			if (!set.gBufferShader || !set.mergeShader)
			{
				// TODO : assert info.flags is valid
				// TODO : projection of info.flags
				// TODO : assert info.flags has a corresponding shader

				if (!set.gBufferShader)
				{
					set.gBufferShader = _shaderMap[info.type].find(info.flags)->second.gBufferShader;
				}
				if (!set.mergeShader)
				{
					set.mergeShader = _shaderMap[info.type].find(info.flags)->second.mergeShader;
				}
			}

			drawSequence.insert({ set, drawable });
		}
	}

	void Scene::_loadShaders()
	{
		spl::ShaderModule modules[] = {
			{ spl::ShaderStage::Vertex,		meshVert,	sizeof(meshVert) },
			{ spl::ShaderStage::Fragment,	meshFrag,	sizeof(meshFrag) }
		};

		std::array<const spl::ShaderModule*, 5> moduleArray;

		moduleArray = { modules + 0, modules + 1, nullptr, nullptr, nullptr };
		_shaders.push_back(new spl::ShaderProgram(moduleArray.data(), 2));
		// etc...


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
					{ DrawableFlags::None, { _shaders[0], nullptr } }
					// etc...
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
