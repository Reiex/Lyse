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
	class Renderbuffer : public FramebufferAttachable
	{
		public:

			Renderbuffer() = delete;
			Renderbuffer(uint32_t width, uint32_t height, TextureInternalFormat internalFormat, uint32_t samples = 0);
			Renderbuffer(const Renderbuffer& renderbuffer) = delete;
			Renderbuffer(Renderbuffer&& renderbuffer) = delete;

			Renderbuffer& operator=(const Renderbuffer& renderbuffer) = delete;
			Renderbuffer& operator=(Renderbuffer&& renderbuffer) = delete;

			bool isValid() const;
			uint32_t getHandle() const;
			const scp::u32vec2& getSize() const;
			TextureInternalFormat getInternalFormat() const;
			uint32_t getSampleCount() const;

			virtual ~Renderbuffer() override;

		private:

			uint32_t _renderbuffer;
			scp::u32vec2 _size;
			TextureInternalFormat _internalFormat;
			uint32_t _samples;
	};
}
