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
	class Renderbuffer
	{
		public:

			Renderbuffer() = delete;
			Renderbuffer(TextureInternalFormat internalFormat, uint32_t width, uint32_t height, uint32_t samples = 0);
			Renderbuffer(const Renderbuffer& renderbuffer) = delete;
			Renderbuffer(Renderbuffer&& renderbuffer) = delete;

			Renderbuffer& operator=(const Renderbuffer& renderbuffer) = delete;
			Renderbuffer& operator=(Renderbuffer&& renderbuffer) = delete;

			uint32_t getHandle() const;
			TextureInternalFormat getInternalFormat() const;
			const scp::u32vec2& getSize() const;

			~Renderbuffer();

		private:

			uint32_t _renderbuffer;
			TextureInternalFormat _internalFormat;
			scp::u32vec2 _size;
	};
}
