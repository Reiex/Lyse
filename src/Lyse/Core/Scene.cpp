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
	Scene::Scene(uint32_t width, uint32_t height) :
		_framebuffer(),
		_shader(),
		_camera(nullptr),
		_drawables()
	{
		spl::ShaderModule modules[] = {
			{ spl::ShaderStage::Vertex,		mainVert,	sizeof(mainVert) - 1},
			{ spl::ShaderStage::Fragment,	mainFrag,	sizeof(mainFrag) - 1}
		};

		_shader.createFromShaderModules(modules, 2);

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

	void Scene::addDrawable(const SceneDrawable* drawable)
	{
		_drawables.insert(drawable);
	}

	void Scene::removeDrawable(const SceneDrawable* drawable)
	{
		_drawables.erase(drawable);
	}

	void Scene::render() const
	{
		spl::Context* context = spl::Context::getCurrentContext();
		const spl::Framebuffer* contextFramebuffer = context->getFramebufferBinding(spl::FramebufferTarget::DrawFramebuffer);
		const spl::ShaderProgram* contextShader = context->getShaderBinding();

		spl::Framebuffer::bind(_framebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::Framebuffer::clear();

		spl::ShaderProgram::bind(_shader);
		_shader.setUniform("projection", dynamic_cast<const CameraPerspective*>(_camera)->getProjectionMatrix());
		_shader.setUniform("view", _camera->getViewMatrix());
		_shader.setUniform("cameraPos", _camera->getTranslation());

		for (const SceneDrawable* drawable : _drawables)
		{
			drawable->draw(_shader, 1.f);
		}

		spl::Framebuffer::bind(*contextFramebuffer, spl::FramebufferTarget::DrawFramebuffer);
		spl::ShaderProgram::bind(*contextShader);
	}

	const spl::Texture2D& Scene::getTexture() const
	{
		return dynamic_cast<const spl::Texture2D&>(*_framebuffer.getTextureAttachment(spl::FramebufferAttachment::ColorAttachment0));
	}

	bool Scene::isValid() const
	{
		return _camera != nullptr;
	}
}
