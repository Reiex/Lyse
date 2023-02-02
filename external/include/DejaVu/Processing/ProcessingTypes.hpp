///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Core/CoreTypes.hpp>

namespace djv
{
	namespace proc
	{
		template<typename T> concept CPrPixel = CPixel<T> && std::floating_point<typename T::ComponentType>;

		template<CPrPixel TPixel> class PrImage;
		template<typename T> concept CPrImage = requires { typename T::PixelType; } && CPrPixel<typename T::PixelType> && std::derived_from<T, PrImage<typename T::PixelType>>;
	}
}
