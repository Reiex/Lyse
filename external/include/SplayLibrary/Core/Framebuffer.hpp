///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>

namespace spl
{
	enum class FramebufferTarget
	{
		DrawFramebuffer,
		ReadFramebuffer
	};

	enum class FramebufferAttachment
	{
		DepthAttachment,
		StencilAttachment,
		DepthStencilAttachment,
		ColorAttachment0,
		ColorAttachment1,
		ColorAttachment2,
		ColorAttachment3,
		ColorAttachment4,
		ColorAttachment5,
		ColorAttachment6,
		ColorAttachment7,
		ColorAttachment8,
		ColorAttachment9,
		ColorAttachment10,
		ColorAttachment11,
		ColorAttachment12,
		ColorAttachment13,
		ColorAttachment14,
		ColorAttachment15,
		ColorAttachment16,
		ColorAttachment17,
		ColorAttachment18,
		ColorAttachment19,
		ColorAttachment20,
		ColorAttachment21,
		ColorAttachment22,
		ColorAttachment23,
		ColorAttachment24,
		ColorAttachment25,
		ColorAttachment26,
		ColorAttachment27,
		ColorAttachment28,
		ColorAttachment29,
		ColorAttachment30,
		ColorAttachment31
	};

	class Framebuffer
	{
		public:

			Framebuffer();
			Framebuffer(const Framebuffer& framebuffer) = delete;
			Framebuffer(Framebuffer&& framebuffer) = delete;

			Framebuffer& operator=(const Framebuffer& framebuffer) = delete;
			Framebuffer& operator=(Framebuffer&& framebuffer) = delete;

			template<typename TextureType, typename... Args> void createNewTextureAttachment(FramebufferAttachment attachment, Args&&... args);
			virtual const TextureBase* getTextureAttachment(FramebufferAttachment attachment) const;
			virtual void createNewRenderBufferAttachment(FramebufferAttachment attachment, TextureInternalFormat internalFormat, const scp::u32vec2& size, uint32_t samples = 0);
			const RenderBuffer* getRenderBufferAttachment(FramebufferAttachment attachment) const;
			void removeAttachment(FramebufferAttachment attachment);

			uint32_t getHandle() const;

			static void bind(const Framebuffer& framebuffer, FramebufferTarget target);
			static void clear(bool color = true, bool depth = true, bool stencil = true);

			virtual ~Framebuffer();

		protected:

			Framebuffer(Window* window);

		private:

			void _attachTexture(FramebufferAttachment attachment);

			uint32_t _framebuffer;
			std::unordered_map<FramebufferAttachment, TextureBase*> _textureAttachments;
			std::unordered_map<FramebufferAttachment, RenderBuffer*> _renderBufferAttachments;
	};
}
