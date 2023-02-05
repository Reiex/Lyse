///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>
#include <SplayLibrary/Core/Texture/TextureBase.hpp>

namespace spl
{
	class Texture2D : public TextureBase
	{
		public:

			Texture2D();
			Texture2D(const scp::u32vec2& size, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const scp::u32vec2& size, const uint8_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const scp::u32vec2& size, const uint16_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const scp::u32vec2& size, const uint32_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const scp::u32vec2& size, const float* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const std::filesystem::path& path, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			template<djv::CPixel TPixel> Texture2D(const djv::Image<TPixel>& image, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			Texture2D(const Texture2D& texture) = delete;
			Texture2D(Texture2D&& texture) = delete;

			Texture2D& operator=(const Texture2D& texture) = delete;
			Texture2D& operator=(Texture2D&& texture) = delete;

			void createNew(const scp::u32vec2& size, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void createNew(const scp::u32vec2& size, const uint8_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void createNew(const scp::u32vec2& size, const uint16_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void createNew(const scp::u32vec2& size, const uint32_t* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void createNew(const scp::u32vec2& size, const float* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void createNew(const std::filesystem::path& path, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			template<djv::CPixel TPixel> void createNew(const djv::Image<TPixel>& image, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_ni8);
			void update(const scp::u32vec2& size, const uint8_t* data, const scp::u32vec2& offset = { 0, 0 });
			void update(const scp::u32vec2& size, const uint16_t* data, const scp::u32vec2& offset = { 0, 0 });
			void update(const scp::u32vec2& size, const uint32_t* data, const scp::u32vec2& offset = { 0, 0 });
			void update(const scp::u32vec2& size, const float* data, const scp::u32vec2& offset = { 0, 0 });
			void update(const std::filesystem::path& path, const scp::u32vec2& offset = { 0, 0 });
			template<djv::CPixel TPixel> void update(const djv::Image<TPixel>& image, const scp::u32vec2& offset = { 0, 0 });
			void destroy();

			const scp::u32vec2& getSize() const;

			virtual ~Texture2D() override final;

		private:

			scp::u32vec2 _size;
	};
}
