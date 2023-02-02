///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Processing/ProcessingTypes.hpp>

namespace djv
{
	namespace proc
	{
		enum class GradientMethod
		{
			Naive,
			Leap,
			Prewitt,
			Sobel,
			Scharr
		};

		enum class LaplacianMethod
		{
			Naive,
			Diagonals,
		};

		struct Clustering
		{
			constexpr Clustering(uint64_t width, uint64_t height);
			constexpr Clustering(const Clustering& clustering) = default;
			constexpr Clustering(Clustering&& clustering) = default;

			constexpr Clustering& operator=(const Clustering& clustering) = default;
			constexpr Clustering& operator=(Clustering&& clustering) = default;

			template<CPrImage TImage> constexpr void applyTo(TImage& image) const;

			~Clustering() = default;

			scp::Matrix<uint32_t> map;
		};

		template<CPrPixel TPixel>
		class PrImage : public Image<TPixel>
		{
			public:

				using Super = Image<TPixel>;

				using PixelType = Super::PixelType;
				using ComponentType = Super::ComponentType;
				using Super::componentCount;

				using Super::Image;

				// Tensor-like operators

				constexpr PrImage<TPixel>& operator+=(const PrImage<TPixel>& image);
				constexpr PrImage<TPixel>& operator-=(const PrImage<TPixel>& image);
				constexpr PrImage<TPixel>& operator*=(const TPixel& pixel);
				constexpr PrImage<TPixel>& operator/=(const TPixel& pixel);
				constexpr PrImage<TPixel>& operator*=(const ComponentType& value);
				constexpr PrImage<TPixel>& operator/=(const ComponentType& value);
				constexpr void negate();

				constexpr void fft(PrImage<TPixel>* phase = nullptr);
				constexpr void ifft(const PrImage<TPixel>* phase = nullptr);
				constexpr void normalize(const TPixel& min = colors::black<ComponentType, componentCount>, const TPixel& max = colors::white<ComponentType, componentCount>);

				// Differential operators

				template<scp::BorderBehaviour BBehaviour, GradientMethod GMethod> constexpr void gradientX();
				template<scp::BorderBehaviour BBehaviour, GradientMethod GMethod> constexpr void gradientY();
				template<scp::BorderBehaviour BBehaviour, LaplacianMethod LMethod> constexpr void laplacian();

				// Edge detection

				template<scp::BorderBehaviour BBehaviour> constexpr void canny(const PrImage<TPixel>* gx = nullptr, const PrImage<TPixel>* gy = nullptr);
				template<scp::BorderBehaviour BBehaviour> constexpr void marrHildreth(const PrImage<TPixel>* lapl = nullptr);

				// Corner detection

				// Shape detection

				// Clustering

				template<uint32_t IterationMax = UINT32_MAX> constexpr Clustering kMeans(uint32_t clusterCount) const;
				template<uint32_t IterationMax = UINT32_MAX> constexpr Clustering slicSuperpixels(uint32_t superpixelCount, float spatialIntensity) const;

			private:

				using Image<TPixel>::_width;
				using Image<TPixel>::_height;

				using Image<TPixel>::_pixels;
				using Image<TPixel>::_zeroColor;

				using Image<TPixel>::_owner;
		};

		using PrImage_gs_f32 = PrImage<Pixel_gs_f32>;
		using PrImage_rgb_f32 = PrImage<Pixel_rgb_f32>;
		using PrImage_rgba_f32 = PrImage<Pixel_rgba_f32>;
	}
}
