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
	class Texture2D : public Texture
	{
		public:

			Texture2D();
			Texture2D(uint32_t width, uint32_t height, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);
			Texture2D(const std::filesystem::path& path, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);
			template<CColorVecType TColorVec> Texture2D(uint32_t width, uint32_t height, const TColorVec* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);
			Texture2D(const Texture2D& texture) = delete;
			Texture2D(Texture2D&& texture) = delete;

			Texture2D& operator=(const Texture2D& texture) = delete;
			Texture2D& operator=(Texture2D&& texture) = delete;

			void createNew(uint32_t width, uint32_t height, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);
			void createNew(const std::filesystem::path& path, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);
			template<CColorVecType TColorVec> void createNew(uint32_t width, uint32_t height, const TColorVec* data, TextureInternalFormat internalFormat = TextureInternalFormat::RGB_nu8, uint32_t levels = 1);

			virtual ~Texture2D() override final = default;

		private:

			using Texture::createNew;
	};
}
