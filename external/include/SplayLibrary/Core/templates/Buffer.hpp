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
	template<CTextureInternalFormatType TClearValue>
	void Buffer::clear(const TClearValue& clearValue, uintptr_t size, uintptr_t offset)
	{
		constexpr TextureInternalFormat internalFormat = _spl::textureInternalFormatTypeToTextureInternalFormat<TClearValue>();
		constexpr TextureFormat format = _spl::textureInternalFormatToTextureFormat(internalFormat);
		constexpr TextureDataType dataType = _spl::textureInternalFormatToTextureDataType(internalFormat);

		_clear(internalFormat, offset, size, format, dataType, &clearValue, sizeof(TClearValue));
	}
}
