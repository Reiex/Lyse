///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreDecl.hpp>
#include <SplayLibrary/Private/PrivateIncluded.hpp>

namespace spl
{
	namespace _spl
	{
		template<djv::CPixel TPixel>
		void imageFree(void* image)
		{
			delete reinterpret_cast<djv::Image<TPixel>*>(image);
		}
	}

	template<TextureInternalFormat InternalFormat>
	void Texture::_loadImageFromFile(const std::filesystem::path& path, ImageLoadInfos& infos)
	{
		using ColorVecType = _spl::TextureInternalFormatToColorVecType<InternalFormat>::Type;
		using PixelType = _spl::ColorVecTypeToPixelType<ColorVecType>::Type;
		static constexpr TextureInternalFormat projectedFormat = _spl::colorVecTypeToTextureInternalFormat<ColorVecType>();

		djv::Image<PixelType>* specImage = new djv::Image<PixelType>(path);
		specImage->flip<true, false>();	// TODO: Replace it with a flip at load in DejaVu

		infos.format = _spl::textureInternalFormatToTextureFormat(InternalFormat);
		infos.dataType = _spl::textureInternalFormatToTextureDataType(projectedFormat);

		infos.data = specImage->getData();
		infos.width = specImage->getWidth();
		infos.height = specImage->getHeight();

		infos.image = specImage;
		infos.imageFree = &_spl::imageFree<PixelType>;
	}
}
