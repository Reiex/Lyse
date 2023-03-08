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
	class FramebufferAttachable
	{
		protected:

			FramebufferAttachable() = default;
			FramebufferAttachable(const FramebufferAttachable& attachable) = delete;
			FramebufferAttachable(FramebufferAttachable&& attachable) = delete;

			FramebufferAttachable& operator=(const FramebufferAttachable& attachable) = delete;
			FramebufferAttachable& operator=(FramebufferAttachable&& attachable) = delete;

			void _detachFramebuffers();

			virtual ~FramebufferAttachable() = default;

			std::unordered_set<Framebuffer*> _framebufferAttached;

		friend class Framebuffer;
	};
}
