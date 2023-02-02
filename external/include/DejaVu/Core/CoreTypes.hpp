///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SciPP/SciPPTypes.hpp>
#include <Diskon/DiskonTypes.hpp>

#include <DejaVu/stb_image/stb_image.h>
#include <DejaVu/stb_image/stb_image_write.h>

namespace djv
{
	class Shape;
	namespace shape
	{
		class ThickRect;
		class FilledRect;
		class Rect;
		class Crown;
		class Disc;
		class Circle;
		class Line;
	}

	template<typename TComponent, uint8_t ComponentCount> class Pixel;
	template<typename T> concept CPixel = requires { typename T::ComponentType; T::componentCount; } && std::derived_from<T, Pixel<typename T::ComponentType, T::componentCount>>;
	namespace colors
	{
		template<typename TComponent, uint8_t ComponentCount> static constexpr Pixel<TComponent, ComponentCount> black = std::integral<TComponent> ? std::numeric_limits<TComponent>::min() : -1.0;
		template<typename TComponent, uint8_t ComponentCount> static constexpr Pixel<TComponent, ComponentCount> white = std::integral<TComponent> ? std::numeric_limits<TComponent>::max() : 1.0;
	}

	enum class ImageFormat;
	template<CPixel TPixel> class Image;
	template<typename T> concept CImage = requires { typename T::PixelType; } && CPixel<typename T::PixelType> && std::derived_from<T, Image<typename T::PixelType>>;

}
