///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>
#include <SplayLibrary/Core/RawTexture.hpp>

namespace spl
{
	// TODO: Template class that take an internal format and whose "::Type" is a corresponding PixelType
	// template<TextureInternalFormat InternalFormat> struct InternalFormatPixelType { using PixelType = djv::RGBAPixel; }
	// The goal is to load only necessary precision and necessary components

	class TextureBase
	{
		public:

			TextureBase(const TextureBase& texture) = delete;
			TextureBase(TextureBase&& texture) = delete;

			TextureBase& operator=(const TextureBase& texture) = delete;
			TextureBase& operator=(TextureBase&& texture) = delete;

			const RawTexture& getRawTexture() const;
			RawTexture& getRawTexture();
			bool isValid() const;

			virtual ~TextureBase() = default;

		protected:

			TextureBase() = default;

			template<djv::CPixel TPixel> static void _createDejaVuImgBuffer(const djv::Image<TPixel>& image, TextureInternalFormat internalFormat, void*& buffer, TextureFormat& format, TextureDataType& dataType);

			RawTexture _texture;
	};
}
