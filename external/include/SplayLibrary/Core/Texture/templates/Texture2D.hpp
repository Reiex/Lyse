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
	template<CColorVecType TColorVec>
	Texture2D::Texture2D(uint32_t width, uint32_t height, const TColorVec* data, TextureInternalFormat internalFormat, uint32_t levels) : Texture2D()
	{
		createNew<TColorVec>(width, height, data, internalFormat, levels);
	}

	template<CColorVecType TColorVec> void createNew(uint32_t width, uint32_t height, const TColorVec* data, TextureInternalFormat internalFormat, uint32_t levels)
	{
		static constexpr TextureInternalFormat projectedFormat = _spl::colorVecTypeToTextureInternalFormat<TColorVec>();

		TextureCreationParams creationParams;
		creationParams.target = TextureTarget::Texture2D;
		creationParams.internalFormat = internalFormat;
		creationParams.width = width;
		creationParams.height = height;
		creationParams.levels = levels;

		createNew(creationParams);

		TextureUpdateParams updateParams;
		updateParams.data = data;
		updateParams.dataFormat = _spl::textureInternalFormatToTextureFormat(internalFormat);
		updateParams.dataType = _spl::textureInternalFormatToTextureDataType(projectedFormat);
		updateParams.width = width;
		updateParams.height = height;

		update(updateParams);
	}
}
