///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Processing/ProcessingDecl.hpp>

namespace djv
{
	namespace proc
	{
		constexpr Clustering::Clustering(uint64_t width, uint64_t height) :
			map(height, width)
		{
		}

		template<CPrImage TImage>
		constexpr void Clustering::applyTo(TImage& image) const
		{
			using TPixel = TImage::PixelType;

			assert(image.getWidth() == map.getSize(1));
			assert(image.getHeight() == map.getSize(0));

			std::unordered_map<uint32_t, std::pair<TPixel, uint64_t>> clusterColor;

			TPixel* itImage = image.getData();
			const uint32_t* it = map.getData();
			const uint32_t* const itEnd = it + map.getSize(0) * map.getSize(1);

			// Compute mean color of each cluster

			for (; it != itEnd; ++it, ++itImage)
			{
				auto itCluster = clusterColor.find(*it);
				if (itCluster == clusterColor.end())
				{
					clusterColor[*it] = { *itImage, 1 };
				}
				else
				{
					itCluster->second.first += *itImage;
					++itCluster->second.second;
				}
			}

			for (std::pair<const uint32_t, std::pair<TPixel, uint64_t>>& cluster : clusterColor)
			{
				cluster.second.first /= cluster.second.second;
			}

			// Apply these mean colors to the image

			itImage = image.getData();
			it = map.getData();
			for (; it != itEnd; ++it, ++itImage)
			{
				*itImage = clusterColor[*it].first;
			}
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator+=(const PrImage<TPixel>& image)
		{
			assert(_width == image._width);
			assert(_height == image._height);

			TPixel* it = _pixels;
			const TPixel* itImage = image._pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it, ++itImage)
			{
				*it += *itImage;
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator-=(const PrImage<TPixel>& image)
		{
			assert(_width == image._width);
			assert(_height == image._height);

			TPixel* it = _pixels;
			const TPixel* itImage = image._pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it, ++itImage)
			{
				*it -= *itImage;
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator*=(const TPixel& pixel)
		{
			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*it)[k] *= pixel[k];
				}
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator/=(const TPixel& pixel)
		{
			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*it)[k] /= pixel[k];
				}
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator*=(const ComponentType& value)
		{
			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				*it *= value;
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr PrImage<TPixel>& PrImage<TPixel>::operator/=(const ComponentType& value)
		{
			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				*it /= value;
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr void PrImage<TPixel>::negate()
		{
			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*it)[k] = -(*it)[k];
				}
			}

			return *this;
		}

		template<CPrPixel TPixel>
		constexpr void PrImage<TPixel>::fft(PrImage<TPixel>* phase)
		{
			if (phase)
			{
				assert(_width == phase->_width);
				assert(_height == phase->_height);
			}

			scp::Matrix<std::complex<ComponentType>> matrix(_height, _width);

			ComponentType* it;
			ComponentType* itPhase;
			std::complex<ComponentType>* itMatrix;
			const std::complex<ComponentType>* const itMatrixEnd = matrix.getData() + _width * _height;

			for (uint8_t k = 0; k < componentCount; ++k)
			{
				it = reinterpret_cast<ComponentType*>(_pixels) + k;
				itMatrix = matrix.getData();

				for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount)
				{
					*itMatrix = *it;
				}

				matrix.fft();

				it = reinterpret_cast<ComponentType*>(_pixels) + k;
				itMatrix = matrix.getData();
				if (phase)
				{
					itPhase = reinterpret_cast<ComponentType*>(phase->_pixels) + k;
					for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount, itPhase += componentCount)
					{
						*it = std::abs(*itMatrix);
						*itPhase = std::arg(*itMatrix);
					}
				}
				else
				{
					for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount)
					{
						*it = std::abs(*itMatrix);
					}
				}
			}
		}

		template<CPrPixel TPixel>
		constexpr void PrImage<TPixel>::ifft(const PrImage<TPixel>* phase)
		{
			if (phase)
			{
				assert(_width == phase->_width);
				assert(_height == phase->_height);
			}

			scp::Matrix<std::complex<ComponentType>> matrix(_height, _width);

			ComponentType* it;
			const ComponentType* itPhase;
			std::complex<ComponentType>* itMatrix;
			const std::complex<ComponentType>* const itMatrixEnd = matrix.getData() + _width * _height;

			for (uint8_t k = 0; k < componentCount; ++k)
			{
				it = reinterpret_cast<ComponentType*>(_pixels) + k;
				itMatrix = matrix.getData();

				if (phase)
				{
					itPhase = reinterpret_cast<const ComponentType*>(phase->_pixels) + k;
					for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount, itPhase += componentCount)
					{
						*itMatrix = std::polar<ComponentType>(*it, *itPhase);
					}
				}
				else
				{
					for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount)
					{
						*itMatrix = *it;
					}
				}

				matrix.ifft();

				it = reinterpret_cast<ComponentType*>(_pixels) + k;
				itMatrix = matrix.getData();
				for (; itMatrix != itMatrixEnd; ++itMatrix, it += componentCount)
				{
					*it = itMatrix->real();
				}
			}
		}

		template<CPrPixel TPixel>
		constexpr void PrImage<TPixel>::normalize(const TPixel& min, const TPixel& max)
		{
			TPixel minComp = _pixels[0];
			TPixel maxComp = _pixels[0];

			TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			for (; it != itEnd; ++it)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					if ((*it)[k] < minComp[k])
					{
						minComp[k] = (*it)[k];
					}
					else if ((*it)[k] > maxComp[k])
					{
						maxComp[k] = (*it)[k];
					}
				}
			}

			TPixel coeff;
			for (uint8_t k = 0; k < componentCount; ++k)
			{
				coeff[k] = (max[k] - min[k]) / (maxComp[k] - minComp[k]);
			}

			it = _pixels;
			for (; it != itEnd; ++it)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*it)[k] = coeff[k] * ((*it)[k] - minComp[k]) + min[k];
				}
			}
		}

		template<CPrPixel TPixel>
		template<scp::BorderBehaviour BBehaviour, GradientMethod GMethod>
		constexpr void PrImage<TPixel>::gradientX()
		{
			assert(_width >= 2 && _height >= 2);

			const uint64_t preWidth = _width - 1;
			const uint64_t preHeight = _height - 1;

			TPixel* it = _pixels;
			if constexpr (GMethod == GradientMethod::Naive)
			{
				const TPixel* itRight = nullptr;

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it)
					{
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);

						*it = (*itRight - *it) / 2;
					}
				}
			}
			else if constexpr (GMethod == GradientMethod::Leap)
			{
				TPixel* itLeft = nullptr;
				const TPixel* itRight = nullptr;

				TPixel tmp;

				for (uint64_t j = 0; j < _height; ++j)
				{
					DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, 0, j);
					DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, 0, j);

					tmp = (*itRight - *itLeft) / 2;
					++it;

					for (uint64_t i = 1; i < _width; ++i, ++it)
					{
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);

						std::swap(*itLeft, tmp);
						tmp = (*itRight - tmp) / 2;
					}
				}
			}
			else if constexpr (GMethod == GradientMethod::Prewitt)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itLeft = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itRight = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itRightUp + *itRight + *itRightDown - *itLeftUp - *itLeft - *itLeftDown) / 6;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (GMethod == GradientMethod::Sobel)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itLeft = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itRight = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itRightUp + *itRight * 2 + *itRightDown - *itLeftUp - *itLeft * 2 - *itLeftDown) / 8;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (GMethod == GradientMethod::Scharr)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itLeft = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itRight = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (3 * (*itRightUp + *itRightDown - *itLeftUp - *itLeftDown) + 10 * (*itRight - *itLeft)) / 32;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
		}

		template<CPrPixel TPixel>
		template<scp::BorderBehaviour BBehaviour, GradientMethod GMethod>
		constexpr void PrImage<TPixel>::gradientY()
		{
			assert(_width >= 2 && _height >= 2);

			const uint64_t preWidth = _width - 1;
			const uint64_t preHeight = _height - 1;

			TPixel* it = _pixels;
			if constexpr (GMethod == GradientMethod::Naive)
			{
				const TPixel* itDown = nullptr;

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it)
					{
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);

						*it = (*itDown - *it) / 2;
					}
				}
			}
			else if constexpr (GMethod == GradientMethod::Leap)
			{
				const TPixel* itUp = nullptr;
				const TPixel* itDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itDown - *itUp) / 2;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (GMethod == GradientMethod::Prewitt)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itUp = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itDown = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itLeftDown + *itDown + *itRightDown - *itLeftUp - *itUp - *itRightUp) / 6;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (GMethod == GradientMethod::Sobel)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itUp = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itDown = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itLeftDown + *itDown * 2 + *itRightDown - *itLeftUp - *itUp * 2 - *itRightUp) / 6;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (GMethod == GradientMethod::Scharr)
			{
				const TPixel* itLeftUp = nullptr;
				const TPixel* itUp = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itDown = nullptr;
				const TPixel* itRightDown = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);

						*scanline = (3 * (*itLeftDown + *itRightDown - *itLeftUp - *itRightUp) + 10 * (*itDown - *itUp)) / 32;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
		}

		template<CPrPixel TPixel>
		template<scp::BorderBehaviour BBehaviour, LaplacianMethod LMethod>
		constexpr void PrImage<TPixel>::laplacian()
		{
			assert(_width >= 2 && _height >= 2);

			const uint64_t preWidth = _width - 1;
			const uint64_t preHeight = _height - 1;

			TPixel* it = _pixels;
			if constexpr (LMethod == LaplacianMethod::Naive)
			{
				const TPixel* itRight = nullptr;
				const TPixel* itLeft = nullptr;
				const TPixel* itDown = nullptr;
				const TPixel* itUp = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itRight + *itLeft + *itDown + *itUp - *it * 4) / 8;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
			else if constexpr (LMethod == LaplacianMethod::Diagonals)
			{
				const TPixel* itRight = nullptr;
				const TPixel* itLeft = nullptr;
				const TPixel* itDown = nullptr;
				const TPixel* itUp = nullptr;
				const TPixel* itRightDown = nullptr;
				const TPixel* itLeftDown = nullptr;
				const TPixel* itRightUp = nullptr;
				const TPixel* itLeftUp = nullptr;

				TPixel* scanline = new TPixel[_width];
				TPixel* scanlineUp = new TPixel[_width];

				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++scanline)
					{
						DJV_PIXEL_RIGHT(BBehaviour, it, itRight, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT(BBehaviour, it, itLeft, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_DOWN(BBehaviour, it, itDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_UP(BBehaviour, it, itUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_DOWN(BBehaviour, it, itRightDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_DOWN(BBehaviour, it, itLeftDown, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_RIGHT_UP(BBehaviour, it, itRightUp, _width, _height, preWidth, preHeight, i, j);
						DJV_PIXEL_LEFT_UP(BBehaviour, it, itLeftUp, _width, _height, preWidth, preHeight, i, j);

						*scanline = (*itRightDown + *itLeftDown + *itRightUp + *itLeftUp + 2 * (*itRight + *itLeft + *itDown + *itUp) - *it * 12) / 24;
					}

					scanline -= _width;

					if (j != 0)
					{
						std::copy_n(scanlineUp, _width, it - 2 * _width);
					}

					std::swap(scanline, scanlineUp);
				}

				std::copy_n(scanlineUp, _width, it - _width);

				delete[] scanline;
				delete[] scanlineUp;
			}
		}

		template<CPrPixel TPixel>
		template<scp::BorderBehaviour BBehaviour>
		constexpr void PrImage<TPixel>::canny(const PrImage<TPixel>* gx, const PrImage<TPixel>* gy)
		{
			assert(_width >= 2 && _height >= 2);
			if (gx)
			{
				assert(_width == gx->_width);
				assert(_height == gx->_height);
			}
			if (gy)
			{
				assert(_width == gy->_width);
				assert(_height == gy->_height);
			}


			bool deleteGx = (gx == nullptr);
			bool deleteGy = (gy == nullptr);

			if (!gx)
			{
				PrImage<TPixel>* copy = new PrImage<TPixel>(*this);
				copy->gradientX<BBehaviour, GradientMethod::Scharr>();
				gx = copy;
			}

			if (!gy)
			{
				PrImage<TPixel>* copy = new PrImage<TPixel>(*this);
				copy->gradientY<BBehaviour, GradientMethod::Scharr>();
				gy = copy;
			}

			constexpr ComponentType a = 0.41421356237;
			constexpr ComponentType b = 2.41421356237;

			ComponentType* it = reinterpret_cast<ComponentType*>(_pixels);
			const ComponentType* itX = reinterpret_cast<const ComponentType*>(gx->_pixels);
			const ComponentType* itY = reinterpret_cast<const ComponentType*>(gy->_pixels);

			for (uint64_t j = 0; j < _height; ++j)
			{
				for (uint64_t i = 0; i < _width; ++i)
				{
					for (uint8_t k = 0; k < componentCount; ++k, ++it, ++itX, ++itY)
					{
						const ComponentType& x = *itX;
						const ComponentType& y = *itY;

						const ComponentType* xNext = nullptr;
						const ComponentType* yNext = nullptr;
						const ComponentType* xPrevious = nullptr;
						const ComponentType* yPrevious = nullptr;

						if (y <= a * x && y > -a * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i + 1, j)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i + 1, j)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i - 1, j)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i - 1, j)[k];
						}
						else if (y <= b * x && y > a * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i + 1, j + 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i + 1, j + 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i - 1, j - 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i - 1, j - 1)[k];
						}
						else if (y > -b * x && y > b * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i, j + 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i, j + 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i, j - 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i, j - 1)[k];
						}
						else if (y <= -b * x && y > -a * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i - 1, j + 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i - 1, j + 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i + 1, j - 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i + 1, j - 1)[k];
						}
						else if (y <= -a * x && y > a * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i - 1, j)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i - 1, j)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i + 1, j)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i + 1, j)[k];
						}
						else if (y <= a * x && y > b * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i - 1, j - 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i - 1, j - 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i + 1, j + 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i + 1, j + 1)[k];
						}
						else if (y <= b * x && y <= -b * x)
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i, j - 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i, j - 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i, j + 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i, j + 1)[k];
						}
						else
						{
							xNext = &gx->getOutOfBound<BBehaviour>(i + 1, j - 1)[k];
							yNext = &gy->getOutOfBound<BBehaviour>(i + 1, j - 1)[k];
							xPrevious = &gx->getOutOfBound<BBehaviour>(i - 1, j + 1)[k];
							yPrevious = &gy->getOutOfBound<BBehaviour>(i - 1, j + 1)[k];
						}

						ComponentType mag = x * x + y * y;
						ComponentType magNext = (*xNext) * (*xNext) + (*yNext) * (*yNext);
						ComponentType magPrevious = (*xPrevious) * (*xPrevious) + (*yPrevious) * (*yPrevious);

						if (mag > magNext && mag >= magPrevious)
						{
							*it = mag - 1;
						}
						else
						{
							*it = -1;
						}
					}
				}
			}

			if (deleteGx)
			{
				delete gx;
			}

			if (deleteGy)
			{
				delete gy;
			}
		}

		template<CPrPixel TPixel>
		template<scp::BorderBehaviour BBehaviour>
		constexpr void PrImage<TPixel>::marrHildreth(const PrImage<TPixel>* lapl)
		{
			assert(_width >= 2 && _height >= 2);
			if (lapl)
			{
				assert(_width == lapl->_width);
				assert(_height == lapl->_height);
			}
			
			bool deleteLapl = (lapl == nullptr);
			
			if (!lapl)
			{
				PrImage<TPixel>* copy = new PrImage<TPixel>(*this);
				copy->laplacian<BBehaviour, LaplacianMethod::Diagonals>();
				lapl = copy;
			}

			const uint64_t preWidth = _width - 1;
			const uint64_t preHeight = _height - 1;
			
			TPixel* it = _pixels;
			const TPixel* itLapl = lapl->_pixels;

			const TPixel* itRight = nullptr;
			const TPixel* itDown = nullptr;
			
			for (uint64_t j = 0; j < _height; ++j)
			{
				for (uint64_t i = 0; i < _width; ++i, ++it, ++itLapl)
				{
					DJV_PIXEL_RIGHT(BBehaviour, itLapl, itRight, _width, _height, preWidth, preHeight, i, j);
					DJV_PIXEL_DOWN(BBehaviour, itLapl, itDown, _width, _height, preWidth, preHeight, i, j);

					for (uint8_t k = 0; k < componentCount; ++k)
					{
						if ((*itLapl)[k] * (*itRight)[k] < 0)
						{
							(*it)[k] = std::abs((*itLapl)[k] - (*itRight)[k]) - 1;
						}
						else if ((*itLapl)[k] * (*itDown)[k] < 0)
						{
							(*it)[k] = std::abs((*itLapl)[k] - (*itDown)[k]) - 1;
						}
						else
						{
							(*it)[k] = -1;
						}
					}
				}
			}
			
			if (deleteLapl)
			{
				delete lapl;
			}
		}

		template<CPrPixel TPixel>
		template<uint32_t IterationMax>
		constexpr Clustering PrImage<TPixel>::kMeans(uint32_t clusterCount) const
		{
			// TODO: Optimize this with an "octree" in 'componentCount' dimensions

			Clustering clustering(_width, _height);

			// Init cluster colors at random

			TPixel* colors = reinterpret_cast<TPixel*>(alloca(sizeof(TPixel) * clusterCount));
			const TPixel* const colorsEnd = colors + clusterCount;

			for (; colors != colorsEnd; ++colors)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					(*colors)[k] = 2.f * static_cast<float>(rand()) / RAND_MAX - 1.f;
				}
			}
			colors -= clusterCount;

			// K-Means algorithm

			const TPixel* it = _pixels;
			const TPixel* const itEnd = _pixels + _width * _height;

			float* distances = reinterpret_cast<float*>(alloca(sizeof(float) * clusterCount));

			std::pair<TPixel, uint64_t>* newColors = reinterpret_cast<std::pair<TPixel, uint64_t>*>(alloca(sizeof(std::pair<TPixel, uint64_t>) * clusterCount));
			std::fill_n(newColors, clusterCount, std::make_pair<TPixel, uint64_t>(0.f, 0));

			uint32_t iter = 0;
			bool colorsUpdated = true;
			while (colorsUpdated && iter < IterationMax)
			{
				colorsUpdated = false;
				++iter;

				//  For each pixel, compute closest cluster, and add pixel color to new cluster color

				for (it = _pixels; it != itEnd; ++it)
				{
					float minDist = FLT_MAX;
					uint32_t minIndex = 0;
					for (uint32_t c = 0; c < clusterCount; ++c, ++colors, ++distances)
					{
						*distances = 0.f;
						for (uint8_t k = 0; k < componentCount; ++k)
						{
							const float diff = (*colors)[k] - (*it)[k];
							*distances += diff * diff;
						}

						if (*distances < minDist)
						{
							minDist = *distances;
							minIndex = c;
						}
					}
					colors -= clusterCount;
					distances -= clusterCount;

					newColors[minIndex].first += *it;
					++newColors[minIndex].second;
				}
			
				// Update cluster colors

				for (; colors != colorsEnd; ++colors, ++newColors)
				{
					if (newColors->second)
					{
						const TPixel newColor = newColors->first / newColors->second;
						colorsUpdated |= (newColor != *colors);
						*colors = newColor;
					}
					newColors->first = 0.f;
					newColors->second = 0;
				}
				colors -= clusterCount;
				newColors -= clusterCount;
			}

			// Compute cluster belonging of each pixel for the final clusters

			uint32_t* itMap = clustering.map.getData();
			for (it = _pixels; it != itEnd; ++it, ++itMap)
			{
				float minDist = FLT_MAX;
				*itMap = 0;
				for (uint32_t c = 0; c < clusterCount; ++c, ++colors, ++distances)
				{
					*distances = 0.f;
					for (uint8_t k = 0; k < componentCount; ++k)
					{
						const float diff = (*colors)[k] - (*it)[k];
						*distances += diff * diff;
					}

					if (*distances < minDist)
					{
						minDist = *distances;
						*itMap = c;
					}
				}
				colors -= clusterCount;
				distances -= clusterCount;
			}

			return clustering;
		}

		namespace _proc
		{
			template<CPrPixel TPixel>
			struct SlicSuperpixel
			{
				scp::i64vec2 center;
				TPixel color;
			};
		}

		template<CPrPixel TPixel>
		template<uint32_t IterationMax>
		constexpr Clustering PrImage<TPixel>::slicSuperpixels(uint32_t superpixelCount, float spatialIntensity) const
		{
			Clustering clustering(_width, _height);

			// Create superpixels and initialize them (random colors but grid-like centers)

			std::vector<_proc::SlicSuperpixel<TPixel>> superpixels(superpixelCount);

			const uint32_t countX = std::sqrt(_width * superpixelCount / _height);
			const uint32_t countY = std::sqrt(_height * superpixelCount / _width);
			const uint32_t remainder = superpixelCount - countX * countY;

			uint32_t s = 0;
			if (countX != 0 && countY != 0)
			{
				const float stepX = static_cast<float>(_width) / countX;
				const float stepY = static_cast<float>(_height) / countY;

				for (float x = stepX / 2; x < _width; x += stepX)
				{
					for (float y = stepY / 2; y < _height; y += stepY, ++s)
					{
						superpixels[s].center.x = x;
						superpixels[s].center.y = y;
					}
				}
			}

			for (uint32_t i = 0; i < remainder; ++i, ++s)
			{
				superpixels[s].center.x = rand() % _width;
				superpixels[s].center.y = rand() % _height;
			}

			for (_proc::SlicSuperpixel<TPixel>& superpixel : superpixels)
			{
				for (uint8_t k = 0; k < componentCount; ++k)
				{
					superpixel.color[k] = 2.f * static_cast<float>(rand()) / RAND_MAX - 1.f;
				}
			}

			// SLIC algorithm

			spatialIntensity = 2.f * spatialIntensity * spatialIntensity / _height;

			const uint64_t patchRadiusX = 2 * _width / (countX + 1);
			const uint64_t patchRadiusY = 2 * _height / (countY + 1);
			scp::Matrix<float> distances({ _height, _width });
			std::vector<uint64_t> superpixelSizes(superpixelCount);
			std::vector<_proc::SlicSuperpixel<TPixel>> newSuperpixels(superpixelCount);

			uint32_t iter = 0;
			bool superpixelsUpdated = true;
			while (superpixelsUpdated && iter < IterationMax)
			{
				superpixelsUpdated = false;
				++iter;

				// Compute superpixel associated to each pixel

				s = 0;
				distances.fill(FLT_MAX);
				for (const _proc::SlicSuperpixel<TPixel>& superpixel : superpixels)
				{
					const int64_t xBegin = std::max<int64_t>(superpixel.center.x - patchRadiusX, 0);
					const int64_t xEnd = std::min<int64_t>(superpixel.center.x + patchRadiusX, _width);
					const int64_t yBegin = std::max<int64_t>(superpixel.center.y - patchRadiusY, 0);
					const int64_t yEnd = std::min<int64_t>(superpixel.center.y + patchRadiusY, _height);

					for (int64_t y = yBegin; y < yEnd; ++y)
					{
						uint64_t index = y * _width + xBegin;
						for (int64_t x = xBegin; x < xEnd; ++x, ++index)
						{
							float d = 0.f;
							for (uint8_t k = 0; k < componentCount; ++k)
							{
								const float dc = _pixels[index][k] - superpixel.color[k];
								d += dc * dc;
							}

							const float dx = superpixel.center.x - x;
							const float dy = superpixel.center.y - y;

							d += (dx * dx + dy * dy) * spatialIntensity;

							if (d < distances[index])
							{
								distances[index] = d;
								clustering.map[index] = s;
							}
						}
					}

					++s;
				}

				// Compute new superpixel color and position

				std::fill_n(superpixelSizes.begin(), superpixelCount, 0);
				for (_proc::SlicSuperpixel<TPixel>& newSuperpixel : newSuperpixels)
				{
					newSuperpixel.center = 0;
					newSuperpixel.color = 0;
				}

				const TPixel* it = _pixels;
				const uint32_t* itMap = clustering.map.getData();
				for (uint64_t j = 0; j < _height; ++j)
				{
					for (uint64_t i = 0; i < _width; ++i, ++it, ++itMap)
					{
						newSuperpixels[*itMap].center.x += i;
						newSuperpixels[*itMap].center.y += j;
						newSuperpixels[*itMap].color += *it;
						++superpixelSizes[*itMap];
					}
				}

				for (s = 0; s < superpixelCount; ++s)
				{
					if (superpixelSizes[s])
					{
						newSuperpixels[s].center /= superpixelSizes[s];
						newSuperpixels[s].color /= superpixelSizes[s];

						superpixelsUpdated = superpixelsUpdated || (newSuperpixels[s].center != superpixels[s].center || newSuperpixels[s].color != superpixels[s].color);
						superpixels[s] = newSuperpixels[s];
					}
				}
			}

			return clustering;
		}
	}
}
