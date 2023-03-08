///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>
#include <SplayLibrary/Core/Framebuffer.hpp>

namespace spl
{
	class DefaultFramebuffer : public Framebuffer
	{
		public:

			DefaultFramebuffer() = delete;
			DefaultFramebuffer(const DefaultFramebuffer& framebuffer) = delete;
			DefaultFramebuffer(DefaultFramebuffer&& framebuffer) = delete;

			DefaultFramebuffer& operator=(const DefaultFramebuffer& framebuffer) = delete;
			DefaultFramebuffer& operator=(DefaultFramebuffer&& framebuffer) = delete;

		private:

			DefaultFramebuffer(Window* window);

			using Framebuffer::attachRenderbuffer;
			using Framebuffer::attachTexture;
			
			virtual ~DefaultFramebuffer() override final = default;

		friend class Window;
	};
}
