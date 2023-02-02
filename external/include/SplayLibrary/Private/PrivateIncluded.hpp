///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/SplayLibraryTypes.hpp>

namespace spl
{
	namespace _spl
	{
		constexpr bool isIndexedBufferTarget(BufferTarget target);

		template<CGlslScalarType TScalar> consteval GlslType glslScalarTypeToGlslType();
		template<CGlslVecType TVec> consteval GlslType glslVecTypeToGlslType();
		template<CGlslMatType TMat> consteval GlslType glslMatTypeToGlslType();

		template<CTextureInternalFormatType TValue> consteval TextureInternalFormat textureInternalFormatTypeToTextureInternalFormat();
		constexpr TextureFormat textureInternalFormatToTextureFormat(TextureInternalFormat internalFormat);
		constexpr TextureDataType textureInternalFormatToTextureDataType(TextureInternalFormat internalFormat);
		constexpr uint8_t textureInternalFormatToComponentCount(TextureInternalFormat internalFormat);

		constexpr BufferUsage drawableStorageToBufferUsage(DrawableStorage storage);
		constexpr BufferStorageFlags::Flags drawableStorageToBufferStorageFlags(DrawableStorage storage);
	}
}

#include <SplayLibrary/Private/templates/PrivateIncluded.hpp>
