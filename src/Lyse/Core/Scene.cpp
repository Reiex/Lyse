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
	thread_local std::unordered_map<DrawableType, std::unordered_map<uint64_t, spl::ShaderProgram*>> Scene::_shaderMap;

	Scene::Scene(uint32_t width, uint32_t height) :
		_framebuffer(),
		_camera(nullptr),
		_drawables()
	{
		if (++_sceneCount == 1)
		{
			_loadShaders();
		}

		_framebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height });
		_framebuffer.createNewRenderbufferAttachment(spl::FramebufferAttachment::DepthStencilAttachment, spl::TextureInternalFormat::Depth_nu24_Stencil_u8, width, height);
	}

	void Scene::resize(uint32_t width, uint32_t height)
	{
		_framebuffer.createNewTextureAttachment<spl::Texture2D>(spl::FramebufferAttachment::ColorAttachment0, scp::u32vec2{ width, height });
		_framebuffer.createNewRenderbufferAttachment(spl::FramebufferAttachment::DepthStencilAttachment, spl::TextureInternalFormat::Depth_nu24_Stencil_u8, width, height);
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
			const spl::Framebuffer* framebuffer;
			const spl::ShaderProgram* shader;

			void save();
			void restore() const;
		};

		void ContextSave::save()
		{
			spl::Context* context = spl::Context::getCurrentContext();

			framebuffer = context->getFramebufferBinding(spl::FramebufferTarget::DrawFramebuffer);
			shader = context->getShaderBinding();
		}

		void ContextSave::restore() const
		{
			spl::Framebuffer::bind(*framebuffer, spl::FramebufferTarget::DrawFramebuffer);
			spl::ShaderProgram::bind(*shader);
		}
	}

	void Scene::render() const
	{
		// Save OpenGL context - `render` must not appear to modify context from caller pov

		ContextSave contextSave;
		contextSave.save();

		spl::Framebuffer::bind(_framebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear();

		// Compute draw sequence - The less shader binding, the better

		std::multimap<const spl::ShaderProgram*, const Drawable*> drawSequence;
		for (const Drawable* drawable : _drawables)
		{
			_insertInDrawSequence(drawSequence, drawable);
		}

		// Draw elements

		DrawContext drawContext;
		drawContext.program = nullptr;
		drawContext.transform = 1.f;
		
		for (const std::pair<const spl::ShaderProgram*, const Drawable*>& elt : drawSequence)
		{
			if (elt.first != drawContext.program)
			{
				drawContext.program = elt.first;

				spl::ShaderProgram::bind(*drawContext.program);

				drawContext.program->setUniform("projection", dynamic_cast<const CameraPerspective*>(_camera)->getProjectionMatrix());
				drawContext.program->setUniform("view", _camera->getViewMatrix());
				drawContext.program->setUniform("cameraPos", _camera->getTranslation());
			}

			elt.second->_draw(drawContext);
		}

		// Restore OpenGL context

		contextSave.restore();
	}

	const spl::Texture2D& Scene::getTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_framebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	bool Scene::isValid() const
	{
		return _camera != nullptr;
	}

	void Scene::_insertInDrawSequence(std::multimap<const spl::ShaderProgram*, const Drawable*>& drawSequence, const Drawable* drawable)
	{
		const DrawableInfo& info = drawable->_getInfo();

		if (info.type == DrawableType::Group)
		{
			const DrawableGroup* group = dynamic_cast<const DrawableGroup*>(drawable);
			assert(group != nullptr);

			uint32_t childCount = group->_getChildCount();
			for (uint32_t i = 0; i < childCount; ++i)
			{
				_insertInDrawSequence(drawSequence, group->_getChild(i));
			}
		}
		else
		{
			const spl::ShaderProgram* shader = info.shader;

			if (shader == nullptr)
			{
				// TODO : assert info.flags is valid
				// TODO : projection of info.flags
				// TODO : assert info.flags has a corresponding shader

				shader = _shaderMap[info.type].find(info.flags)->second;
			}

			drawSequence.insert({ shader, drawable });
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
					{ DrawableFlags::None, _shaders[0] }
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
