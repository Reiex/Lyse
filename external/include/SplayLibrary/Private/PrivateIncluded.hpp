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

		template<TextureInternalFormat InternalFormat> struct TextureInternalFormatToColorVecType { using Type = void; };
		template<CColorVecType TColorVec> struct ColorVecTypeToPixelType { using Type = void; };
		template<CColorVecType TColorVec> consteval TextureInternalFormat colorVecTypeToTextureInternalFormat();
		constexpr TextureFormat textureInternalFormatToTextureFormat(TextureInternalFormat internalFormat);
		constexpr TextureDataType textureInternalFormatToTextureDataType(TextureInternalFormat internalFormat);
	}
}

#include <SplayLibrary/Private/templates/PrivateIncluded.hpp>
