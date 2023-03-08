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
		ColorAttachment
	};

	class Framebuffer
	{
		public:

			Framebuffer();
			Framebuffer(const Framebuffer& framebuffer) = delete;
			Framebuffer(Framebuffer&& framebuffer) = delete;

			Framebuffer& operator=(const Framebuffer& framebuffer) = delete;
			Framebuffer& operator=(Framebuffer&& framebuffer) = delete;

			void attachRenderbuffer(FramebufferAttachment attachment, uint32_t attachmentIndex, Renderbuffer* renderbuffer);
			void attachTexture(FramebufferAttachment attachment, uint32_t attachmentIndex, Texture* texture, uint32_t layer = -1, uint32_t level = 0);

			bool isValid() const;
			uint32_t getHandle() const;

			virtual ~Framebuffer();

			static void bind(FramebufferTarget target, const Framebuffer* framebuffer);

			static void clear(bool color = true, bool depth = true, bool stencil = true);

		protected:

			Framebuffer(Window* window);

		private:

			void _removeAttachment(const FramebufferAttachable* attachment);
			void _updateDrawBuffers();

			uint32_t _framebuffer;
			std::vector<FramebufferAttachable*> _attachments;

		friend class FramebufferAttachable;
	};
}
