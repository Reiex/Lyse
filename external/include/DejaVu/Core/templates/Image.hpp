///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Core/CoreDecl.hpp>

namespace djv
{
	namespace _djv
	{
		template<uint8_t ComponentCount>
		constexpr void defaultLoadSwizzling(uint8_t* swizzling)
		{
			constexpr uint8_t n = std::min<uint8_t>(ComponentCount, 4);
			for (uint8_t i = 0; i < n; ++i)
			{
				swizzling[i] = i;
			}

			if constexpr (ComponentCount > 4)
			{
				std::fill_n(swizzling + 4, ComponentCount - 4, UINT8_MAX);
			}
		}

		template<uint8_t ComponentCount>
		constexpr void defaultSaveSwizzling(uint8_t* swizzling)
		{
			constexpr uint8_t n = std::min<uint8_t>(ComponentCount, 4);
			for (uint8_t i = 0; i < n; ++i)
			{
				swizzling[i] = i;
			}

			if constexpr (ComponentCount < 4)
			{
				std::fill_n(swizzling + ComponentCount, 4 - ComponentCount, UINT8_MAX);
			}
		}
	
		template<CPixel TPixelFrom, CPixel TPixelTo>
		constexpr void defaultConversionFunction(const TPixelFrom& from, TPixelTo& to)
		{
			constexpr uint8_t n = std::min(TPixelFrom::componentCount, TPixelTo::componentCount);
			for (uint8_t i = 0; i < n; ++i)
			{
				to.set(i, from[i]);
			}
		}
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image() :
		_width(0),
		_height(0),
		_pixels(nullptr),
		_zeroColor(colors::black<ComponentType, componentCount>),
		_owner(true)
	{
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(uint64_t width, uint64_t height) : Image<TPixel>()
	{
		createNew(width, height);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(uint64_t width, uint64_t height, const TPixel& value) : Image<TPixel>()
	{
		createNew(width, height, value);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(uint64_t width, uint64_t height, const TPixel* values) : Image<TPixel>()
	{
		createNew(width, height, values);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::filesystem::path& path) : Image<TPixel>()
	{
		createFromFile(path);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::filesystem::path& path, const uint8_t* swizzling) : Image<TPixel>()
	{
		createFromFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling) : Image<TPixel>()
	{
		createFromFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::istream& stream, ImageFormat format) : Image<TPixel>()
	{
		createFromStream(stream, format);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::istream& stream, ImageFormat format, const uint8_t* swizzling) : Image<TPixel>()
	{
		createFromStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling) : Image<TPixel>()
	{
		createFromStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	template<CImage TImage>
	constexpr Image<TPixel>::Image(const TImage& image) : Image<TPixel>()
	{
		createFromConversion(image);
	}

	template<CPixel TPixel>
	template<CImage TImage>
	constexpr Image<TPixel>::Image(const TImage& image, const PixelConversionFunction<typename TImage::PixelType, TPixel>& conversionFunc) : Image<TPixel>()
	{
		createFromConversion(image, conversionFunc);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const Image<TPixel>& image, uint64_t width, uint64_t height, scp::InterpolationMethod method) : Image<TPixel>()
	{
		switch (method)
		{
			case scp::InterpolationMethod::Nearest:
				createFromResize<scp::InterpolationMethod::Nearest>(image, width, height);
				break;
			case scp::InterpolationMethod::Linear:
				createFromResize<scp::InterpolationMethod::Linear>(image, width, height);
				break;
			case scp::InterpolationMethod::Cubic:
				createFromResize<scp::InterpolationMethod::Cubic>(image, width, height);
				break;
			default:
				assert(false);
				break;
		}
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const Image<TPixel>& image, uint64_t x, uint64_t y, uint64_t width, uint64_t height) : Image<TPixel>()
	{
		createFromCrop(image, x, y, width, height);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(const Image<TPixel>& image) : Image<TPixel>()
	{
		_copyFrom(image);
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::Image(Image<TPixel>&& image) : Image<TPixel>()
	{
		_moveFrom(std::forward<Image<TPixel>>(image));
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>* Image<TPixel>::constructAroundMemory(uint64_t width, uint64_t height, TPixel* memory)
	{
		assert(width != 0);
		assert(height != 0);

		Image<TPixel>* image = new Image<TPixel>();

		image._width = width;
		image._height = height;
		image._pixels = memory;
		image._owner = false;

		return image;
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>& Image<TPixel>::operator=(const Image<TPixel>& image)
	{
		_copyFrom(image);
		return *this;
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>& Image<TPixel>::operator=(Image<TPixel>&& image)
	{
		_moveFrom(std::forward<Image<TPixel>>(image));
		return *this;
	}


	template<CPixel TPixel>
	constexpr void Image<TPixel>::createNew(uint64_t width, uint64_t height)
	{
		assert(width != 0);
		assert(height != 0);

		if (width != _width || height != _height)
		{
			_destroy();
			_create(width, height);
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createNew(uint64_t width, uint64_t height, const TPixel& value)
	{
		createNew(width, height);
		std::fill_n(_pixels, _width * _height, value);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createNew(uint64_t width, uint64_t height, const TPixel* values)
	{
		createNew(width, height);
		std::copy_n(_pixels, _width * _height, values);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromFile(const std::filesystem::path& path)
	{
		uint8_t swizzling[componentCount];
		_djv::defaultLoadSwizzling<componentCount>(swizzling);
		_createFromFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromFile(const std::filesystem::path& path, const uint8_t* swizzling)
	{
		_createFromFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromFile(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling)
	{
		assert(swizzling.size() == componentCount);
		_createFromFile(path, swizzling.begin());
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromStream(const std::istream& stream, ImageFormat format)
	{
		uint8_t swizzling[componentCount];
		_djv::defaultLoadSwizzling<componentCount>(swizzling);
		_createFromStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromStream(const std::istream& stream, ImageFormat format, const uint8_t* swizzling)
	{
		_createFromStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromStream(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling)
	{
		assert(swizzling.size() == componentCount);
		_createFromStream(stream, format, swizzling.begin());
	}

	template<CPixel TPixel>
	template<CImage TImage> constexpr void Image<TPixel>::createFromConversion(const TImage& image)
	{
		createFromConversion(image, _djv::defaultConversionFunction<typename TImage::PixelType, TPixel>);
	}

	template<CPixel TPixel>
	template<CImage TImage> constexpr void Image<TPixel>::createFromConversion(const TImage& image, const PixelConversionFunction<typename TImage::PixelType, TPixel>& conversionFunc)
	{
		createNew(image._width, image._height);

		TPixel* it = _pixels;
		const TPixel* const itEnd = it + _width * _height;
		const typename TImage::PixelType* itFrom = image._pixels;

		for (; it != itEnd; ++it, ++itFrom)
		{
			conversionFunc(*itFrom, *it);
		}
	}

	template<CPixel TPixel>
	template<scp::InterpolationMethod IMethod> constexpr void Image<TPixel>::createFromResize(const Image<TPixel>& image, uint64_t width, uint64_t height)
	{
		createNew(width, height);

		// Simple copy

		if (image._width == _width && image._height == _height)
		{
			_copyFrom(image);
		}

		// Downsample

		else if (image._width >= _width && image._width % _width == 0 && image._height >= _height && image._height % _height == 0)
		{
			const uint64_t xStep = image._width / _width;
			const uint64_t yStep = image._height / _height - 1;

			TPixel* it = _pixels;
			const TPixel* itImage = image._pixels;

			for (uint64_t i = 0; i < _height; ++i)
			{
				for (uint64_t j = 0; j < _width; ++j, ++it, itImage += xStep)
				{
					*it = *itImage;
				}

				itImage += yStep * image._width;
			}
		}

		else
		{
			if constexpr (IMethod == scp::InterpolationMethod::Nearest)
			{
				// Upsample

				if (_width > image._width && _width % image._width == 0 && _height > image._height && _height % image._height == 0)
				{
					const uint64_t xRepeat = _width / image._width;
					const uint64_t yRepeat = _height / image._height - 1;

					TPixel* it = _pixels;
					const TPixel* itImage = image._pixels;

					for (uint64_t i = 0; i < image._height; ++i)
					{
						for (uint64_t j = 0; j < image._width; ++j, ++itImage, it += xRepeat)
						{
							std::fill_n(it, xRepeat, *itImage);
						}

						for (uint64_t k = 0; k < yRepeat; ++k, it += _width)
						{
							std::copy(it - _width, it, it);
						}
					}

					return;
				}

				// X-Downsample & Y-Upsample

				else if (image._width > _width && image._width % _width == 0 && _height > image._height && _height % image._height == 0)
				{
					const uint64_t xStep = image._width / _width;
					const uint64_t yRepeat = _height / image._height - 1;

					TPixel* it = _pixels;
					const TPixel* itImage = image._pixels;

					for (uint64_t i = 0; i < image._height; ++i)
					{
						for (uint64_t j = 0; j < _width; ++j, ++it, itImage += xStep)
						{
							*it = *itImage;
						}

						for (uint64_t k = 0; k < yRepeat; ++k, it += _width)
						{
							std::copy(it - _width, it, it);
						}
					}

					return;
				}

				// X-Upsample & Y-Downsample

				else if (_width > image._width && _width % image._width == 0 && image._height > _height && image._height % _height == 0)
				{
					const uint64_t xRepeat = _width / image._width;
					const uint64_t yStep = image._height / _height - 1;

					TPixel* it = _pixels;
					const TPixel* itImage = image._pixels;

					for (uint64_t i = 0; i < _height; ++i)
					{
						for (uint64_t j = 0; j < image._width; ++j, ++itImage, it += xRepeat)
						{
							std::fill_n(it, xRepeat, *itImage);
						}

						itImage += yStep * image._width;
					}

					return;
				}
			}

			// Pure resize (with interpolation...)

			scp::Tensor<TPixel>* imageTensor = scp::Tensor<TPixel>::createAroundMemory({ image._height, image._width }, const_cast<TPixel*>(image._pixels));
			scp::Tensor<TPixel>* tensor = scp::Tensor<TPixel>::createAroundMemory({ _height, _width }, _pixels);

			tensor->interpolation<float, IMethod>(*imageTensor);

			delete imageTensor;
			delete tensor;
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::createFromCrop(const Image<TPixel>& image, uint64_t x, uint64_t y, uint64_t width, uint64_t height)
	{
		assert(x + width <= image._width);
		assert(y + height <= image._height);

		createNew(width, height);

		const TPixel* itImage = image._pixels + y * image._width + x;
		TPixel* it = _pixels;

		for (uint64_t j = 0; j < _height; ++j, it += _width, itImage += image._width)
		{
			std::copy_n(itImage, _width, it);
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToFile(const std::filesystem::path& path) const
	{
		uint8_t swizzling[4];
		_djv::defaultSaveSwizzling<componentCount>(swizzling);
		_saveToFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToFile(const std::filesystem::path& path, const uint8_t* swizzling) const
	{
		_saveToFile(path, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToFile(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling) const
	{
		assert(swizzling.size() == 4);
		_saveToFile(path, swizzling.begin());
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToStream(const std::istream& stream, ImageFormat format)
	{
		uint8_t swizzling[4];
		_djv::defaultSaveSwizzling<componentCount>(swizzling);
		_saveToStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToStream(const std::istream& stream, ImageFormat format, const uint8_t* swizzling)
	{
		_saveToStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::saveToStream(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling)
	{
		assert(swizzling.size() == 4);
		_saveToStream(stream, format, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::transpose()
	{
		scp::Matrix<TPixel>* matrix = scp::Matrix<TPixel>::createAroundMemory(_height, _width, _pixels);
		matrix->transpose();
		delete matrix;

		std::swap(_width, _height);
	}

	template<CPixel TPixel>
	template<scp::InterpolationMethod IMethod, scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::rotate(float angle)
	{
		if (std::abs(angle - 0.f) < 1e-3)
		{
			// Nothing.
		}
		else if (std::abs(angle - 0.5f * std::numbers::pi) < 1e-3)
		{
			transpose();
			flip<true, false>();
		}
		else if (std::abs(angle - std::numbers::pi) < 1e-3)
		{
			flip<true, true>();
		}
		else if (std::abs(angle - 1.5f * std::numbers::pi) < 1e-3)
		{
			flip<true, false>();
			transpose();
		}
		else
		{
			const float w = static_cast<float>(_width);
			const float h = static_cast<float>(_height);
			const float ca = std::cos(angle);
			const float sa = -std::sin(angle);
			const float ar = -angle + std::atan(h / w);
			const float car = std::cos(ar);
			const float sar = std::sin(ar);
			const float r = std::sqrt(w * w + h * h);
			const float mx = std::min({ 0.f, -h * sa, w * ca, r * car });
			const float my = std::min({ 0.f, h * ca, w * sa, r * sar });
			const float Mx = std::max({ 0.f, -h * sa, w * ca, r * car });
			const float My = std::max({ 0.f, h * ca, w * sa, r * sar });

			scp::Tensor<TPixel>* tensor = scp::Tensor<TPixel>::createAroundMemory({ _height, _width }, _pixels);

			_width = Mx - mx + 1;
			_height = My - my + 1;

			TPixel* pixels = new TPixel[_width * _height];
			TPixel* it = pixels;

			float indices[2];
			for (uint64_t j = 0; j < _height; ++j)
			{
				for (uint64_t i = 0; i < _width; ++i, ++it)
				{
					indices[0] = (j + my) * ca - (i + mx) * sa;
					indices[1] = (i + mx) * ca + (j + my) * sa;

					if (indices[0] >= 0 && indices[0] < h && indices[1] >= 0 && indices[1] < w)
					{
						*it = tensor->getInterpolated<float, IMethod>(indices);
					}
					else if constexpr (BBehaviour == scp::BorderBehaviour::Zero)
					{
						*it = _zeroColor;
					}
					else
					{
						*it = tensor->getOutOfBound<BBehaviour>({ static_cast<int64_t>(indices[0]), static_cast<int64_t>(indices[1]) });
					}
				}
			}

			delete tensor;
			if (_owner)
			{
				delete[] _pixels;
			}
			_pixels = pixels;
		}
	}

	template<CPixel TPixel>
	template<bool Vertically, bool Horizontally>
	constexpr void Image<TPixel>::flip()
	{
		uint64_t index;
		uint64_t revIndex;

		if constexpr (Vertically && Horizontally)
		{
			index = 0;
			revIndex = _width * _height - 1;
			for (; index < revIndex; ++index, --revIndex)
			{
				std::swap(_pixels[index], _pixels[revIndex]);
			}
		}
		else if constexpr (Vertically && !Horizontally)
		{
			const uint64_t halfHeight = _height / 2;

			index = 0;
			for (uint64_t j = 0; j < halfHeight; ++j)
			{
				revIndex = (_height - j - 1) * _width;
				for (uint64_t i = 0; i < _width; ++i, ++index, ++revIndex)
				{
					std::swap(_pixels[index], _pixels[revIndex]);
				}
			}
		}
		else if constexpr (!Vertically && Horizontally)
		{
			for (uint64_t j = 0; j < _height; ++j)
			{
				index = j * _width;
				revIndex = (j + 1) * _width - 1;
				for (; index < revIndex; ++index, --revIndex)
				{
					std::swap(_pixels[index], _pixels[revIndex]);
				}
			}
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::draw(const Shape& shape, const TPixel& color)
	{
		auto it = shape.getPixelIterator();

		int64_t x, y;
		while (it->getPixel(x, y))
		{
			if (x >= 0 && y >= 0 && x < _width && y < _height)
			{
				_pixels[x * _height + y] = color;
			}
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::draw(const Shape& shape, const Image<TPixel>& image)
	{
		// TODO: Rewrite draw
		/*
			- This function only allows to draw a shape composed of "image" into "this".
			- The part of the image drawn should be offsetable...
			- Pixels out of image but in shape should be drawn using BBehaviour/_zeroColor
		*/

		auto it = shape.getPixelIterator();

		int64_t x, y;
		while (it->getPixel(x, y))
		{
			if (x >= 0 && y >= 0 && x < _width && y < _height && x < image._width && y < image._height)
			{
				_pixels[y * _width + x] = image._pixels[y * image._width + x];
			}
		}
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurGaussian(float sigma)
	{
		blurGaussian<BBehaviour>(sigma, sigma);
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurGaussian(float sigmaX, float sigmaY)
	{
		const int64_t rx = sigmaX * 2.57;
		const int64_t ry = sigmaY * 2.57;
		const uint64_t dx = 2 * rx + 1;
		const uint64_t dy = 2 * ry + 1;

		float acc[componentCount];

		// Compute gaussian weights

		float gaussianFactorX = 0.f;
		float* weightsX = reinterpret_cast<float*>(alloca(sizeof(float) * dx));
		for (int64_t i = 0, x = -rx; i < dx; ++i, ++x)
		{
			const float ratio = x / sigmaX;
			weightsX[i] = std::exp(-ratio * ratio / 2);
			gaussianFactorX += weightsX[i];
		}
		for (int64_t i = 0, x = -rx; i < dx; ++i, ++x)
		{
			weightsX[i] /= gaussianFactorX;
		}

		float gaussianFactorY = 0.f;
		float* weightsY = reinterpret_cast<float*>(alloca(sizeof(float) * dy));
		for (int64_t i = 0, y = -ry; i < dy; ++i, ++y)
		{
			const float ratio = y / sigmaY;
			weightsY[i] = std::exp(-ratio * ratio / 2);
			gaussianFactorY += weightsY[i];
		}
		for (int64_t i = 0, y = -ry; i < dy; ++i, ++y)
		{
			weightsY[i] /= gaussianFactorY;
		}

		// Compute gaussian blur horizontally

		TPixel* scanline = new TPixel[std::max(_width, _height)];

		TPixel* it = _pixels;
		for (int64_t j = 0; j < _height; ++j)
		{
			for (int64_t i = 0; i < _width; ++i, ++scanline)
			{
				std::fill_n(acc, componentCount, 0.f);
				for (int64_t p = 0, x = i - rx; p < dx; ++p, ++x)
				{
					const TPixel& pixel = getOutOfBound<BBehaviour>(x, j);
					for (uint8_t k = 0; k < componentCount; ++k)
					{
						acc[k] += pixel[k] * weightsX[p];
					}
				}

				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*scanline)[k] = acc[k];
				}
			}

			scanline -= _width;
			std::copy_n(scanline, _width, it);
			it += _width;
		}

		// Compute gaussian blur vertically

		const TPixel* const scanlineEnd = scanline;
		for (int64_t i = 0; i < _width; ++i)
		{
			for (int64_t j = 0; j < _height; ++j, ++scanline)
			{
				std::fill_n(acc, componentCount, 0.f);
				for (int64_t q = 0, y = j - ry; q < dy; ++q, ++y)
				{
					const TPixel& pixel = getOutOfBound<BBehaviour>(i, y);
					for (uint8_t k = 0; k < componentCount; ++k)
					{
						acc[k] += pixel[k] * weightsY[q];
					}
				}

				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*scanline)[k] = acc[k];
				}
			}

			it = _pixels + i + _width * (_height - 1);
			for (; scanline != scanlineEnd; it -= _width)
			{
				*it = *(--scanline);
			}
		}

		delete[] scanline;
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurMean(uint64_t radius)
	{
		blurMean<BBehaviour>(radius, radius);
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurMean(uint64_t radiusX, uint64_t radiusY)
	{
		const int64_t rx = radiusX;
		const int64_t ry = radiusY;
		const uint64_t dx = 2 * rx + 1;
		const uint64_t dy = 2 * ry + 1;

		float acc[componentCount];

		// Compute mean blur horizontally

		TPixel* scanline = new TPixel[std::max(_width, _height)];

		TPixel* it = _pixels;
		for (int64_t j = 0; j < _height; ++j)
		{
			std::fill_n(acc, componentCount, 0.f);
			for (int64_t i = -rx; i <= rx; ++i)
			{
				const TPixel& pixel = getOutOfBound<BBehaviour>(i, j);
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					acc[k] += pixel[k];
				}
			}

			for (uint8_t k = 0; k < componentCount; ++k)
			{
				(*scanline)[k] = acc[k] / dx;
			}
			++scanline;

			for (int64_t i = 1, iPreced = -rx, iNext = 1 + rx; i < _width; ++i, ++iPreced, ++iNext, ++scanline)
			{
				const TPixel& pixelPreced = getOutOfBound<BBehaviour>(iPreced, j);
				const TPixel& pixelNext = getOutOfBound<BBehaviour>(iNext, j);
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					acc[k] -= pixelPreced[k];
					acc[k] += pixelNext[k];
					(*scanline)[k] = acc[k] / dx;
				}
			}

			scanline -= _width;
			std::copy_n(scanline, _width, it);
			it += _width;
		}

		// Compute mean blur vertically

		const TPixel* const scanlineEnd = scanline;
		for (int64_t i = 0; i < _width; ++i)
		{
			std::fill_n(acc, componentCount, 0.0);
			for (int64_t j = -ry; j <= ry; ++j)
			{
				const TPixel& pixel = getOutOfBound<BBehaviour>(i, j);
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					acc[k] += pixel[k];
				}
			}

			for (uint8_t k = 0; k < componentCount; ++k)
			{
				(*scanline)[k] = acc[k] / dy;
			}
			++scanline;

			for (int64_t j = 1, jPreced = -ry, jNext = 1 + ry; j < _height; ++j, ++jPreced, ++jNext, ++scanline)
			{
				const TPixel& pixelPreced = getOutOfBound<BBehaviour>(i, jPreced);
				const TPixel& pixelNext = getOutOfBound<BBehaviour>(i, jNext);
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					acc[k] -= pixelPreced[k];
					acc[k] += pixelNext[k];
					(*scanline)[k] = acc[k] / dy;
				}
			}

			it = _pixels + i + _width * (_height - 1);
			for (; scanline != scanlineEnd; it -= _width)
			{
				*it = *(--scanline);
			}
		}

		delete[] scanline;
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurMedian(uint64_t radius)
	{
		blurMedian<BBehaviour>(radius, radius);
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::blurMedian(uint64_t radiusX, uint64_t radiusY)
	{
		const int64_t rx = radiusX;
		const int64_t ry = radiusY;
		const uint64_t dx = 2 * radiusX + 1;
		const uint64_t dy = 2 * radiusY + 1;
		const uint64_t histIndex = (dx * dy) / 2;

		std::deque<const TPixel*> pixelList;

		TPixel* itResult = new TPixel[_width * _height];

		if constexpr (sizeof(TComponent) == 1)
		{
			uint8_t tmp;
			std::array<std::array<uint64_t, 256>, componentCount> histogram;

			for (uint64_t j = 0; j < _height; ++j)
			{
				// Create histogram at start of line

				pixelList.clear();
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					std::fill_n(histogram[k].data(), 256, 0);
				}

				for (int64_t i = -rx; i <= rx; ++i)
				{
					for (int64_t p = 0, y = j - ry; p < dy; ++p, ++y)
					{
						pixelList.push_back(&getOutOfBound<BBehaviour>(i, y));
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							pixelList.back()->get(k, tmp);
							++histogram[k][tmp];
						}
					}
				}

				for (uint8_t k = 0; k < componentCount; ++k)
				{
					uint64_t* it = histogram[k].data();
					uint64_t n = *it;
					while (n < histIndex) { n += *(++it); }
					itResult->set(k, static_cast<uint8_t>(std::distance(histogram[k].data(), it)));
				}
				++itResult;

				// Update histogram along the line

				for (uint64_t i = 1; i < _width; ++i, ++itResult)
				{
					int64_t xBefore = i - rx - 1;
					int64_t xAfter = i + rx;

					for (int64_t p = 0, y = j - ry; p < dy; ++p, ++y)
					{
						pixelList.push_back(&getOutOfBound<BBehaviour>(xAfter, y));
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							pixelList.back()->get(k, tmp);
							++histogram[k][tmp];
							pixelList.front()->get(k, tmp);
							--histogram[k][tmp];
						}
						pixelList.pop_front();
					}

					for (uint8_t k = 0; k < componentCount; ++k)
					{
						uint64_t* it = histogram[k].data();
						uint64_t n = *it;
						while (n < histIndex) { n += *(++it); }
						itResult->set(k, static_cast<uint8_t>(std::distance(histogram[k].data(), it)));
					}
				}
			}
		}
		else
		{
			std::array<std::map<TComponent, uint64_t>, componentCount> histogram;

			for (uint64_t j = 0; j < _height; ++j)
			{
				// Create histogram at start of line

				pixelList.clear();
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					histogram[k].clear();
				}

				for (int64_t i = -rx; i <= rx; ++i)
				{
					for (int64_t p = 0, y = j - ry; p < dy; ++p, ++y)
					{
						pixelList.push_back(&getOutOfBound<BBehaviour>(i, y));
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							auto it = histogram[k].find((*pixelList.back())[k]);
							if (it == histogram[k].end())
							{
								histogram[k].emplace((*pixelList.back())[k], 1);
							}
							else
							{
								++it->second;
							}
						}
					}
				}

				for (uint8_t k = 0; k < componentCount; ++k)
				{
					auto it = histogram[k].begin();
					uint64_t n = it->second;
					while (n < histIndex)
					{
						++it;
						n += it->second;
					}
					(*itResult)[k] = it->first;
				}
				++itResult;

				// Update histogram along the line

				for (uint64_t i = 1; i < _width; ++i, ++itResult)
				{
					int64_t xBefore = i - rx - 1;
					int64_t xAfter = i + rx;

					for (int64_t p = 0, y = j - ry; p < dy; ++p, ++y)
					{
						pixelList.push_back(&getOutOfBound<BBehaviour>(xAfter, y));
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							auto it = histogram[k].find((*pixelList.back())[k]);
							if (it == histogram[k].end())
							{
								histogram[k].emplace((*pixelList.back())[k], 1);
							}
							else
							{
								++it->second;
							}

							it = histogram[k].find((*pixelList.front())[k]);
							--it->second;
							if (it->second == 0)
							{
								histogram[k].erase(it);
							}
						}
						pixelList.pop_front();
					}

					for (uint8_t k = 0; k < componentCount; ++k)
					{
						auto it = histogram[k].begin();
						uint64_t n = it->second;
						while (n < histIndex)
						{
							++it;
							n += it->second;
						}
						(*itResult)[k] = it->first;
					}
				}
			}
		}

		itResult -= _width * _height;
		std::copy_n(itResult, _width * _height, _pixels);
		delete[] itResult;
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::filterGaussianBilateral(float sigmaSpace, float sigmaColor)
	{
		const int64_t r = sigmaSpace * 2.57;
		const uint64_t d = 2 * r + 1;

		float acc[componentCount];
		float value[componentCount];
		float tmp[componentCount];
		float ratio, coeff, coeffSpace, coeffColor;
		const TPixel* pixel;

		TPixel* itResult = new TPixel[_width * _height];
		for (uint64_t j = 0; j < _height; ++j)
		{
			for (uint64_t i = 0; i < _width; ++i, ++itResult)
			{
				pixel = &getOutOfBound<BBehaviour>(i, j);
				ratio = 0.0;
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					acc[k] = 0.0;
					pixel->get(k, value[k]);
				}

				for (int64_t q = 0, y = j - r; q < d; ++q, ++y)
				{
					for (int64_t p = 0, x = i - r; p < d; ++p, ++x)
					{
						coeffColor = 0.0;
						coeffSpace = ((x - i) * (x - i) + (y - j) * (y - j)) / (2.0 * sigmaSpace);

						pixel = &getOutOfBound<BBehaviour>(x, y);
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							pixel->get(k, tmp[k]);
							coeffColor += (tmp[k] - value[k]) * (tmp[k] - value[k]);
						}

						coeffColor /= (8.0 * sigmaColor);
						coeff = std::exp(-coeffSpace - coeffColor);

						for (uint8_t k = 0; k < componentCount; ++k)
						{
							acc[k] += tmp[k] * coeff;
						}

						ratio += coeff;
					}
				}

				for (uint8_t k = 0; k < componentCount; ++k)
				{
					itResult->set(k, acc[k] / ratio);
				}
			}
		}

		itResult -= _width * _height;
		std::copy_n(itResult, _width * _height, _pixels);
		delete[] itResult;
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::filterKuwahara(uint64_t radius)
	{
		filterKuwahara<BBehaviour>(radius, radius);
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr void Image<TPixel>::filterKuwahara(uint64_t radiusX, uint64_t radiusY)
	{
		// TODO: implement it.
	}

	template<CPixel TPixel>
	constexpr bool Image<TPixel>::operator==(const Image<TPixel>& image) const
	{
		return _width == image._width && _height == image._height && std::equal(_pixels, _pixels + _width * _height, image._pixels);
	}

	template<CPixel TPixel>
	constexpr bool Image<TPixel>::operator!=(const Image<TPixel>& image) const
	{
		return !operator==(image);
	}

	template<CPixel TPixel>
	constexpr TPixel& Image<TPixel>::operator[](const std::initializer_list<uint64_t>& indices)
	{
		assert(indices.size() == 2);
		assert(indices.begin()[0] < _width && indices.begin()[1] < _height);

		return _pixels[indices.begin()[1] * _width + indices.begin()[0]];
	}

	template<CPixel TPixel>
	constexpr const TPixel& Image<TPixel>::operator[](const std::initializer_list<uint64_t>& indices) const
	{
		assert(indices.size() == 2);
		assert(indices.begin()[0] < _width && indices.begin()[1] < _height);

		return _pixels[indices.begin()[1] * _width + indices.begin()[0]];
	}

	template<CPixel TPixel>
	template<scp::BorderBehaviour BBehaviour>
	constexpr const TPixel& Image<TPixel>::getOutOfBound(int64_t x, int64_t y) const
	{
		if constexpr (BBehaviour == scp::BorderBehaviour::Zero)
		{
			if (x < 0 || x >= _width || y < 0 || y >= _height)
			{
				return _zeroColor;
			}
			else
			{
				return _pixels[y * _width + x];
			}
		}
		else if constexpr (BBehaviour == scp::BorderBehaviour::Continuous)
		{
			uint64_t ux = x & -(x > 0);
			ux = (ux | -(ux >= _width)) & ((_width - 1) | -(ux < _width));

			uint64_t uy = y & -(y > 0);
			uy = (uy | -(uy >= _height)) & ((_height - 1) | -(uy < _height));

			return _pixels[uy * _width + ux];
		}
		else if constexpr (BBehaviour == scp::BorderBehaviour::Periodic)
		{
			uint64_t ux = x;
			if (x < 0 || x >= _width)
			{
				ux = (x % _width) + (_width & -(x < 0));
			}

			uint64_t uy = y;
			if (y < 0 || y >= _height)
			{
				uy = (y % _height) + (_height & -(y < 0));
			}

			return _pixels[uy * _width + ux];
		}
	}

	template<CPixel TPixel>
	constexpr TPixel* Image<TPixel>::begin()
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr TPixel* Image<TPixel>::end()
	{
		return _pixels + _width * _height;
	}

	template<CPixel TPixel>
	constexpr const TPixel* Image<TPixel>::begin() const
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr const TPixel* Image<TPixel>::end() const
	{
		return _pixels + _width * _height;
	}

	template<CPixel TPixel>
	constexpr const TPixel* Image<TPixel>::cbegin() const
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr const TPixel* Image<TPixel>::cend() const
	{
		return _pixels + _width * _height;
	}

	template<CPixel TPixel>
	constexpr const uint64_t& Image<TPixel>::getWidth() const
	{
		return _width;
	}

	template<CPixel TPixel>
	constexpr const uint64_t& Image<TPixel>::getHeight() const
	{
		return _height;
	}

	template<CPixel TPixel>
	constexpr TPixel* Image<TPixel>::getData()
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr const TPixel* Image<TPixel>::getData() const
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr bool Image<TPixel>::isValid() const
	{
		return _pixels;
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::setZeroColor(const TPixel& color)
	{
		_zeroColor = color;
	}

	template<CPixel TPixel>
	constexpr const TPixel& Image<TPixel>::getZeroColor() const
	{
		return _zeroColor;
	}

	template<CPixel TPixel>
	constexpr Image<TPixel>::~Image()
	{
		_destroy();
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_create(uint64_t width, uint64_t height)
	{
		_width = width;
		_height = height;
		_pixels = new TPixel[_width * _height];
		_owner = true;
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_copyFrom(const Image<TPixel>& image)
	{
		if (_width != image._width || _height != image._height)
		{
			_destroy();
			_create(image._width, image._height);
		}

		std::copy_n(image._pixels, _width * _height, _pixels);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_moveFrom(Image<TPixel>&& image)
	{
		if (!_owner || !image._owner)
		{
			_copyFrom(image);
			return;
		}

		_destroy();

		_width = image._width;
		_height = image._height;
		_pixels = image._pixels;
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_destroy()
	{
		if (_pixels && _owner)
		{
			delete[] _pixels;
		}

		_width = 0;
		_height = 0;
		_pixels = nullptr;
		_owner = true;
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_createFromFile(const std::filesystem::path& path, const uint8_t* swizzling)
	{
		if (!std::filesystem::exists(path))
		{
			return;
		}

		std::ifstream stream(path, std::ios::in | std::ios::binary);
		if (!stream)
		{
			return;
		}

		ImageFormat imageFormat;
		if (_extensionToImageFormat(path.extension(), imageFormat))
		{
			_createFromStream(stream, imageFormat, swizzling);
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_createFromStream(std::istream& stream, ImageFormat format, const uint8_t* swizzling)
	{
		assert(stream);

		for (uint8_t i = 0; i < componentCount; ++i)
		{
			assert(swizzling[i] < 4 || swizzling[i] == UINT8_MAX);
		}

		(this->*(_imageFormatToLoadFunc[static_cast<size_t>(format)]))(stream, swizzling);
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_createFromPng(std::istream& stream, const uint8_t* swizzling)
	{
		stream.seekg(0, std::ios_base::end);
		uint64_t length = stream.tellg();
		stream.seekg(0);
		uint8_t* buffer = new uint8_t[length];
		stream.read((char*) buffer, length);

		int w, h, channels;
		uint8_t* img = stbi_load_from_memory(buffer, length, &w, &h, &channels, 4);

		createNew(w, h);

		const uint64_t n = _width * _height;
		for (uint64_t i = 0; i < n; ++i)
		{
			for (uint8_t j = 0; j < componentCount; ++j)
			{
				if (swizzling[j] == 255)
				{
					_pixels[i].set(j, 0);
				}
				else
				{
					_pixels[i].set(j, img[i*4 + swizzling[j]]);
				}
			}
		}

		stbi_image_free(img);
		delete[] buffer;
	}

	template<CPixel TPixel>
	template<ImageFormat Format>
	constexpr void Image<TPixel>::_createFromPnm(std::istream& stream, const uint8_t* swizzling)
	{
		dsk::fmt::PnmIStream pnmIStream;
		pnmIStream.setSource(stream);

		dsk::fmt::pnm::Header pnmHeader;
		pnmIStream.readHeader(pnmHeader);

		switch (Format)
		{
			case ImageFormat::Pbm:
			{
				if (pnmHeader.format != dsk::fmt::pnm::Format::PlainPBM && pnmHeader.format != dsk::fmt::pnm::Format::RawPBM)
				{
					return;
				}

				break;
			}
			case ImageFormat::Pgm:
			{
				if (pnmHeader.format != dsk::fmt::pnm::Format::PlainPGM && pnmHeader.format != dsk::fmt::pnm::Format::RawPGM)
				{
					return;
				}

				break;
			}
			case ImageFormat::Ppm:
			{
				if (pnmHeader.format != dsk::fmt::pnm::Format::PlainPPM && pnmHeader.format != dsk::fmt::pnm::Format::RawPPM)
				{
					return;
				}

				break;
			}
		}

		createNew(pnmHeader.width, pnmHeader.height);

		const float ratio = 2.f / pnmHeader.maxSampleVal.value();
		const uint64_t n = _width * _height;
		uint16_t buffer[3];

		for (uint64_t i = 0; i < n; ++i)
		{
			pnmIStream.readPixels(buffer, 1);

			for (uint8_t j = 0; j < componentCount; ++j)
			{
				if (swizzling[j] == UINT8_MAX)
				{
					_pixels[i][j] = colors::black<TComponent, componentCount>[j];
				}
				else if (swizzling[j] == 3)
				{
					_pixels[i][j] = colors::white<TComponent, componentCount>[j];
				}
				else
				{
					switch (pnmHeader.format)
					{
						case dsk::fmt::pnm::Format::PlainPBM:
						case dsk::fmt::pnm::Format::RawPBM:
						{
							_pixels[i][j] = buffer[0] ? colors::white<TComponent, componentCount>[j] : colors::black<TComponent, componentCount>[j];
							break;
						}
						case dsk::fmt::pnm::Format::PlainPGM:
						case dsk::fmt::pnm::Format::RawPGM:
						{
							_pixels[i].set<float>(j, buffer[0] * ratio - 1.f);
							break;
						}
						case dsk::fmt::pnm::Format::PlainPPM:
						case dsk::fmt::pnm::Format::RawPPM:
						{
							_pixels[i].set<float>(j, buffer[swizzling[j]] * ratio - 1.f);
							break;
						}
					}
				}
			}
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_saveToFile(const std::filesystem::path& path, const uint8_t* swizzling) const
	{
		std::ofstream stream(path, std::ios::out | std::ios::binary);
		if (!stream)
		{
			return;
		}

		ImageFormat imageFormat;
		if (_extensionToImageFormat(path.extension(), imageFormat))
		{
			_saveToStream(stream, imageFormat, swizzling);
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_saveToStream(std::ostream& stream, ImageFormat format, const uint8_t* swizzling) const
	{
		assert(stream);
		assert(swizzling[0] < componentCount || swizzling[0] == UINT8_MAX);
		assert(swizzling[1] < componentCount || swizzling[1] == UINT8_MAX);
		assert(swizzling[2] < componentCount || swizzling[2] == UINT8_MAX);
		assert(swizzling[3] < componentCount || swizzling[3] == UINT8_MAX);

		(this->*(_imageFormatToSaveFunc[static_cast<size_t>(format)]))(stream, swizzling);
	}

	namespace _djv
	{
		inline void stbWriteFunc(void* context, void* data, int size)
		{
			reinterpret_cast<std::ostream*>(context)->write((char*) data, size);
		}
	}

	template<CPixel TPixel>
	constexpr void Image<TPixel>::_saveToPng(std::ostream& stream, const uint8_t* swizzling) const
	{
		uint8_t* img = new uint8_t[_width * _height * 4];

		const uint64_t n = _width * _height;
		for (uint64_t i = 0; i < n; ++i)
		{
			for (uint8_t j = 0; j < 4; ++j, ++img)
			{
				if (swizzling[j] == 255)
				{
					if (j == 3)
					{
						*img = 255;
					}
					else
					{
						*img = 0;
					}
				}
				else
				{
					_pixels[i].get(swizzling[j], *img);
				}
			}
		}

		img -= _width * _height * 4;

		stbi_write_png_to_func(_djv::stbWriteFunc, &stream, _width, _height, 4, img, _width * 4);

		delete[] img;
	}

	template<CPixel TPixel>
	template<ImageFormat Format>
	constexpr void Image<TPixel>::_saveToPnm(std::ostream& stream, const uint8_t* swizzling) const
	{
		dsk::fmt::PnmOStream pnmOStream;
		pnmOStream.setDestination(stream);

		const uint64_t n = _width * _height;

		dsk::fmt::pnm::Header pnmHeader;
		pnmHeader.width = _width;
		pnmHeader.height = _height;

		if constexpr (sizeof(TComponent) == 1)
		{
			pnmHeader.maxSampleVal.emplace(255);
		}
		else
		{
			pnmHeader.maxSampleVal.emplace(65535);
		}

		switch (Format)
		{
			case ImageFormat::Pbm:
			{
				pnmHeader.format = dsk::fmt::pnm::Format::PlainPBM;
				break;
			}
			case ImageFormat::Pgm:
			{
				pnmHeader.format = dsk::fmt::pnm::Format::PlainPGM;
				break;
			}
			case ImageFormat::Ppm:
			{
				pnmHeader.format = dsk::fmt::pnm::Format::PlainPPM;
				break;
			}
			case ImageFormat::Pnm:
			{
				pnmHeader.format = dsk::fmt::pnm::Format::PlainPPM;
				break;
			}
		}

		pnmOStream.writeHeader(pnmHeader);

		uint16_t buffer[3];
		for (uint64_t i = 0; i < n; ++i)
		{
			switch (pnmHeader.format)
			{
				case dsk::fmt::pnm::Format::PlainPBM:
				case dsk::fmt::pnm::Format::PlainPGM:
				{
					if (swizzling[0] == UINT8_MAX)
					{
						buffer[0] = 0;
					}
					else
					{
						if constexpr (sizeof(TComponent) == 1)
						{
							uint8_t tmp;
							_pixels[i].get(swizzling[0], tmp);
							buffer[0] = tmp;
						}
						else
						{
							_pixels[i].get(swizzling[0], buffer[0]);
						}
					}

					break;
				}
				case dsk::fmt::pnm::Format::PlainPPM:
				{
					for (uint8_t j = 0; j < 3; ++j)
					{
						if (swizzling[j] == UINT8_MAX)
						{
							buffer[j] = 0;
						}
						else
						{
							if constexpr (sizeof(TComponent) == 1)
							{
								uint8_t tmp;
								_pixels[i].get(swizzling[j], tmp);
								buffer[j] = tmp;
							}
							else
							{
								_pixels[i].get(swizzling[j], buffer[j]);
							}
						}
					}

					break;
				}
			}

			pnmOStream.writePixels(buffer, 1);
		}
	}

	template<CPixel TPixel>
	constexpr bool Image<TPixel>::_extensionToImageFormat(const std::filesystem::path& extension, ImageFormat& format)
	{
		static const std::unordered_map<std::filesystem::path, ImageFormat> extensionToImageFormat = {
			{ ".png", ImageFormat::Png },
			{ ".pbm", ImageFormat::Pbm },
			{ ".pgm", ImageFormat::Pgm },
			{ ".ppm", ImageFormat::Ppm },
			{ ".pnm", ImageFormat::Pnm }
		};

		auto it = extensionToImageFormat.find(extension);

		if (it == extensionToImageFormat.end())
		{
			return false;
		}
		else
		{
			format = it->second;
			return true;
		}
	}
}
