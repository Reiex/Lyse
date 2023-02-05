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
	enum class ImageFormat
	{
		Png,

		Pbm,
		Pgm,
		Ppm,
		Pnm
	};


	template<CPixel TPixelFrom, CPixel TPixelTo>
	using PixelConversionFunction = std::function<void(const TPixelFrom&, TPixelTo&)>;


	template<CPixel TPixel>
	class Image
	{
		public:
			
			using PixelType = TPixel;
			using ComponentType = typename TPixel::ComponentType;
			static constexpr uint8_t componentCount = TPixel::componentCount;

			constexpr Image(uint64_t width, uint64_t height);
			constexpr Image(uint64_t width, uint64_t height, const TPixel& value);
			constexpr Image(uint64_t width, uint64_t height, const TPixel* values);
			constexpr Image(const std::filesystem::path& path);
			constexpr Image(const std::filesystem::path& path, const uint8_t* swizzling);
			constexpr Image(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling);
			constexpr Image(const std::istream& stream, ImageFormat format);
			constexpr Image(const std::istream& stream, ImageFormat format, const uint8_t* swizzling);
			constexpr Image(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling);
			template<CImage TImage> constexpr Image(const TImage& image);
			template<CImage TImage> constexpr Image(const TImage& image, const PixelConversionFunction<typename TImage::PixelType, TPixel>& conversionFunc);
			constexpr Image(const Image<TPixel>& image, uint64_t width, uint64_t height, scp::InterpolationMethod method);
			constexpr Image(const Image<TPixel>& image, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
			constexpr Image(const Image<TPixel>& image);
			constexpr Image(Image<TPixel>&& image);

			static constexpr Image<TPixel>* constructAroundMemory(uint64_t width, uint64_t height, TPixel* memory);

			constexpr Image<TPixel>& operator=(const Image<TPixel>& image);
			constexpr Image<TPixel>& operator=(Image<TPixel>&& image);

			// Image creation

			constexpr void createNew(uint64_t width, uint64_t height);
			constexpr void createNew(uint64_t width, uint64_t height, const TPixel& value);
			constexpr void createNew(uint64_t width, uint64_t height, const TPixel* values);
			constexpr void createFromFile(const std::filesystem::path& path);
			constexpr void createFromFile(const std::filesystem::path& path, const uint8_t* swizzling);
			constexpr void createFromFile(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling);
			constexpr void createFromStream(const std::istream& stream, ImageFormat format);
			constexpr void createFromStream(const std::istream& stream, ImageFormat format, const uint8_t* swizzling);
			constexpr void createFromStream(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling);
			template<CImage TImage> constexpr void createFromConversion(const TImage& image);
			template<CImage TImage> constexpr void createFromConversion(const TImage& image, const PixelConversionFunction<typename TImage::PixelType, TPixel>& conversionFunc);
			template<scp::InterpolationMethod IMethod> constexpr void createFromResize(const Image<TPixel>& image, uint64_t width, uint64_t height);
			constexpr void createFromCrop(const Image<TPixel>& image, uint64_t x, uint64_t y, uint64_t width, uint64_t height);

			// Image save to format

			constexpr void saveToFile(const std::filesystem::path& path) const;
			constexpr void saveToFile(const std::filesystem::path& path, const uint8_t* swizzling) const;	// {0, 3, -1, -1} means red is first component, green is fourth component, blue is set to 0 and alpha is 255
			constexpr void saveToFile(const std::filesystem::path& path, const std::initializer_list<uint8_t>& swizzling) const;
			constexpr void saveToStream(const std::istream& stream, ImageFormat format);
			constexpr void saveToStream(const std::istream& stream, ImageFormat format, const uint8_t* swizzling);
			constexpr void saveToStream(const std::istream& stream, ImageFormat format, const std::initializer_list<uint8_t>& swizzling);

			// Simple image manipulation

			// TODO: resize
			// TODO: crop
			constexpr void transpose();
			template<scp::InterpolationMethod IMethod, scp::BorderBehaviour BBehaviour> constexpr void rotate(float angle);

			template<bool Vertically, bool Horizontally> constexpr void flip();
			constexpr void draw(const Shape& shape, const TPixel& color);
			constexpr void draw(const Shape& shape, const Image<TPixel>& image);

			// Blurs

			template<scp::BorderBehaviour BBehaviour> constexpr void blurGaussian(float sigma);
			template<scp::BorderBehaviour BBehaviour> constexpr void blurGaussian(float sigmaX, float sigmaY);
			template<scp::BorderBehaviour BBehaviour> constexpr void blurMean(uint64_t radius);
			template<scp::BorderBehaviour BBehaviour> constexpr void blurMean(uint64_t radiusX, uint64_t radiusY);
			template<scp::BorderBehaviour BBehaviour> constexpr void blurMedian(uint64_t radius);
			template<scp::BorderBehaviour BBehaviour> constexpr void blurMedian(uint64_t radiusX, uint64_t radiusY);

			// TODO: other blurs - defocus aberration, directional blur, etc...

			// Denoising filters

			template<scp::BorderBehaviour BBehaviour> constexpr void filterGaussianBilateral(float sigmaSpace, float sigmaColor);
			template<scp::BorderBehaviour BBehaviour> constexpr void filterKuwahara(uint64_t radius);
			template<scp::BorderBehaviour BBehaviour> constexpr void filterKuwahara(uint64_t radiusX, uint64_t radiusY);

			// Accessors

			constexpr bool operator==(const Image<TPixel>& image) const;
			constexpr bool operator!=(const Image<TPixel>& image) const;

			constexpr TPixel& operator[](const std::initializer_list<uint64_t>& indices);
			constexpr const TPixel& operator[](const std::initializer_list<uint64_t>& indices) const;

			template<scp::BorderBehaviour BBehaviour> constexpr const TPixel& getOutOfBound(int64_t x, int64_t y) const;

			constexpr TPixel* begin();
			constexpr TPixel* end();
			constexpr const TPixel* begin() const;
			constexpr const TPixel* end() const;
			constexpr const TPixel* cbegin() const;
			constexpr const TPixel* cend() const;

			constexpr const uint64_t& getWidth() const;
			constexpr const uint64_t& getHeight() const;
			constexpr TPixel* getData();
			constexpr const TPixel* getData() const;
			constexpr bool isValid() const;
			constexpr void setZeroColor(const TPixel& color);
			constexpr const TPixel& getZeroColor() const;

			constexpr ~Image();

		protected:

			using TComponent = typename TPixel::ComponentType;

			constexpr Image();

			constexpr void _create(uint64_t width, uint64_t height);
			constexpr void _copyFrom(const Image<TPixel>& image);
			constexpr void _moveFrom(Image<TPixel>&& image);
			constexpr void _destroy();

			constexpr void _createFromFile(const std::filesystem::path& path, const uint8_t* swizzling);
			constexpr void _createFromStream(std::istream& stream, ImageFormat format, const uint8_t* swizzling);
			
			constexpr void _createFromPng(std::istream& stream, const uint8_t* swizzling);
			template<ImageFormat Format> constexpr void _createFromPnm(std::istream& stream, const uint8_t* swizzling);

			constexpr void _saveToFile(const std::filesystem::path& path, const uint8_t* swizzling) const;
			constexpr void _saveToStream(std::ostream& stream, ImageFormat format, const uint8_t* swizzling) const;

			constexpr void _saveToPng(std::ostream& stream, const uint8_t* swizzling) const;
			template<ImageFormat Format> constexpr void _saveToPnm(std::ostream& stream, const uint8_t* swizzling) const;

			static constexpr void (Image<TPixel>::* _imageFormatToLoadFunc[])(std::istream&, const uint8_t*) = {
				&Image<TPixel>::_createFromPng,
				&Image<TPixel>::_createFromPnm<ImageFormat::Pbm>,
				&Image<TPixel>::_createFromPnm<ImageFormat::Pgm>,
				&Image<TPixel>::_createFromPnm<ImageFormat::Ppm>,
				&Image<TPixel>::_createFromPnm<ImageFormat::Pnm>,
			};
			static constexpr void (Image<TPixel>::* _imageFormatToSaveFunc[])(std::ostream&, const uint8_t*) const = {
				&Image<TPixel>::_saveToPng,
				&Image<TPixel>::_saveToPnm<ImageFormat::Pbm>,
				&Image<TPixel>::_saveToPnm<ImageFormat::Pgm>,
				&Image<TPixel>::_saveToPnm<ImageFormat::Ppm>,
				&Image<TPixel>::_saveToPnm<ImageFormat::Pnm>,
			};
			static constexpr bool _extensionToImageFormat(const std::filesystem::path& extension, ImageFormat& imageFormat);


			uint64_t _width;
			uint64_t _height;

			TPixel* _pixels;
			TPixel _zeroColor;

			bool _owner;

		template<CPixel T> friend class Image;
	};


	using Image_gs_u8 = Image<Pixel_gs_u8>;
	using Image_gs_i8 = Image<Pixel_gs_i8>;
	using Image_gs_u16 = Image<Pixel_gs_u16>;
	using Image_gs_i16 = Image<Pixel_gs_i16>;
	using Image_gs_f32 = Image<Pixel_gs_f32>;
	using Image_rgb_u8 = Image<Pixel_rgb_u8>;
	using Image_rgb_i8 = Image<Pixel_rgb_i8>;
	using Image_rgb_u16 = Image<Pixel_rgb_u16>;
	using Image_rgb_i16 = Image<Pixel_rgb_i16>;
	using Image_rgb_f32 = Image<Pixel_rgb_f32>;
	using Image_rgba_u8 = Image<Pixel_rgba_u8>;
	using Image_rgba_i8 = Image<Pixel_rgba_i8>;
	using Image_rgba_u16 = Image<Pixel_rgba_u16>;
	using Image_rgba_i16 = Image<Pixel_rgba_i16>;
	using Image_rgba_f32 = Image<Pixel_rgba_f32>;
}
