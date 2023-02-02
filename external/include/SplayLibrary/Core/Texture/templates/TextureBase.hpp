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
	template<djv::CPixel TPixel>
	void TextureBase::_createDejaVuImgBuffer(const djv::Image<TPixel>& image, TextureInternalFormat internalFormat, void*& buffer, TextureFormat& format, TextureDataType& dataType)
	{
		format = _spl::textureInternalFormatToTextureFormat(internalFormat);
		dataType = _spl::textureInternalFormatToTextureDataType(internalFormat);
		const uint8_t componentCount = _spl::textureInternalFormatToComponentCount(internalFormat);

		const uint64_t width = image.getWidth();
		const uint64_t height = image.getHeight();
		const uint64_t elementCount = width * height;

		switch (dataType)
		{
			case TextureDataType::UnsignedByte:
				buffer = new uint8_t[elementCount * componentCount];
				break;
			case TextureDataType::Byte:
				buffer = new int8_t[elementCount * componentCount];
				break;
			case TextureDataType::UnsignedShort:
				buffer = new uint16_t[elementCount * componentCount];
				break;
			case TextureDataType::Short:
				buffer = new int16_t[elementCount * componentCount];
				break;
			case TextureDataType::UnsignedInt:
				buffer = new uint32_t[elementCount * componentCount];
				break;
			case TextureDataType::Int:
				buffer = new int32_t[elementCount * componentCount];
				break;
			case TextureDataType::HalfFloat:
				buffer = new float16_t[elementCount * componentCount];
				break;
			case TextureDataType::Float:
				buffer = new float32_t[elementCount * componentCount];
				break;
			case TextureDataType::UnsignedByte_3_3_2:
			case TextureDataType::UnsignedByte_2_3_3_Rev:
				buffer = new uint8_t[elementCount];
				break;
			case TextureDataType::UnsignedShort_5_6_5:
			case TextureDataType::UnsignedShort_5_6_5_Rev:
			case TextureDataType::UnsignedShort_4_4_4_4:
			case TextureDataType::UnsignedShort_4_4_4_4_Rev:
			case TextureDataType::UnsignedShort_5_5_5_1:
			case TextureDataType::UnsignedShort_1_5_5_5_Rev:
				buffer = new uint16_t[elementCount];
				break;
			case TextureDataType::UnsignedInt_8_8_8_8:
			case TextureDataType::UnsignedInt_8_8_8_8_Rev:
			case TextureDataType::UnsignedInt_10_10_10_2:
			case TextureDataType::UnsignedInt_2_10_10_10_Rev:
			case TextureDataType::UnsignedInt_24_8:
			case TextureDataType::UnsignedInt_10F_11F_11F_Rev:
			case TextureDataType::UnsignedInt_5_9_9_9_Rev:
				buffer = new uint32_t[elementCount];
				break;
			case TextureDataType::Float32_UnsignedInt_24_8_Rev:
				buffer = new uint32_t[elementCount * 2];
				break;
			default:
				assert(false);
				return;
		}

		const TPixel* it = image.getData();
		uint64_t k = -1;
		for (uint64_t i = 0; i < elementCount; ++i, ++it)
		{
			switch (dataType)
			{
				case TextureDataType::UnsignedByte:
				{
					uint8_t* data = reinterpret_cast<uint8_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::Byte:
				{
					int8_t* data = reinterpret_cast<int8_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::UnsignedShort:
				{
					uint16_t* data = reinterpret_cast<uint16_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::Short:
				{
					int16_t* data = reinterpret_cast<int16_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::UnsignedInt:
				{
					uint32_t* data = reinterpret_cast<uint32_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::Int:
				{
					int32_t* data = reinterpret_cast<int32_t*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::Float:
				{
					float* data = reinterpret_cast<float*>(buffer);
					switch (format)
					{
						case TextureFormat::R:
						case TextureFormat::iR:
						case TextureFormat::DepthComponent:
						case TextureFormat::StencilIndex:
						{
							it->get(0, data[++k]);
							break;
						}
						case TextureFormat::RG:
						case TextureFormat::iRG:
						case TextureFormat::DepthStencil:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							break;
						}
						case TextureFormat::RGB:
						case TextureFormat::iRGB:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							break;
						}
						case TextureFormat::RGBA:
						case TextureFormat::iRGBA:
						{
							it->get(0, data[++k]);
							it->get(1, data[++k]);
							it->get(2, data[++k]);
							it->get(3, data[++k]);
							break;
						}
						default:
						{
							assert(false);
							break;
						}
					}

					break;
				}
				case TextureDataType::UnsignedByte_3_3_2:
				{
					uint8_t r, g, b;
					it->get(0, r);
					it->get(1, g);
					it->get(2, b);

					uint8_t* data = reinterpret_cast<uint8_t*>(buffer);
					data[++k] = (r & 0xE0) & ((g & 0xE0) >> 3) & ((b & 0xC0) >> 6);
					break;
				}
				case TextureDataType::UnsignedShort_5_6_5:
				{
					uint16_t r, g;
					uint8_t b;
					it->get(0, r);
					it->get(1, g);
					it->get(2, b);

					uint16_t* data = reinterpret_cast<uint16_t*>(buffer);
					data[++k] = (r & 0xF800) & ((g & 0xFC00) >> 5) & ((b & 0xF8) >> 3);
					break;
				}
				case TextureDataType::UnsignedShort_4_4_4_4:
				{
					uint16_t r, g;
					uint8_t b, a;
					it->get(0, r);
					it->get(1, g);
					it->get(2, b);
					it->get(3, a);

					uint16_t* data = reinterpret_cast<uint16_t*>(buffer);
					data[++k] = (r & 0xF000) & ((g & 0xF000) >> 4) & (b & 0xF0) & ((a & 0xF0) >> 4);
					break;
				}
				case TextureDataType::UnsignedShort_5_5_5_1:
				{
					uint16_t r, g;
					uint8_t b, a;
					it->get(0, r);
					it->get(1, g);
					it->get(2, b);
					it->get(3, a);

					uint16_t* data = reinterpret_cast<uint16_t*>(buffer);
					data[++k] = (r & 0xF800) & ((g & 0xF800) >> 5) & ((b & 0xF8) >> 2) & ((a & 0x80) >> 7);
					break;
				}
				case TextureDataType::UnsignedInt_10_10_10_2:
				{
					uint32_t r, g;
					uint16_t b;
					uint8_t a;
					it->get(0, r);
					it->get(1, g);
					it->get(2, b);
					it->get(3, a);

					uint32_t* data = reinterpret_cast<uint32_t*>(buffer);
					data[++k] = (r & 0xFFC00000) & ((g & 0xFFC00000) >> 10) & ((b & 0xFFC0) >> 4) & ((a & 0xC0) >> 6);
					break;
				}
				case TextureDataType::UnsignedInt_24_8:
				{
					uint32_t r;
					uint8_t g;
					it->get(0, r);
					it->get(1, g);

					uint32_t* data = reinterpret_cast<uint32_t*>(buffer);
					data[++k] = (r & 0xFFFFFF00) & g;
					break;
				}
				case TextureDataType::Float32_UnsignedInt_24_8_Rev:
				{
					float* dataFloat = reinterpret_cast<float*>(buffer);
					it->get(0, dataFloat[++k]);
					uint32_t* dataInt = reinterpret_cast<uint32_t*>(buffer);
					it->get(1, dataInt[++k]);
					dataInt[++k] >>= 24;
					break;
				}
				default:
				{
					assert(false);
					break;
				}
			}
		}
	}
}
