///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>

namespace spl
{
	enum class TextureTarget
	{
		Undefined,
		Texture1D,
		Texture2D,
		Texture3D,
		Array1D,
		Array2D,
		Rectangle,
		Buffer,
		CubeMap,
		CubeMapArray,
		Multisample2D,
		Multisample2DArray
	};

	enum class TextureFormat
	{
		Undefined,	///< TODO
		StencilIndex,
		DepthComponent,
		DepthStencil,
		R,
		G,
		B,
		RG,
		RGB,
		RGBA,
		BGR,
		BGRA,
		iR,
		iG,
		iB,
		iRG,
		iRGB,
		iRGBA,
		iBGR,
		iBGRA
	};

	enum class TextureDataType
	{
		Undefined,	///< TODO
		UnsignedByte,
		Byte,
		UnsignedShort,
		Short,
		UnsignedInt,
		Int,
		HalfFloat,
		Float,
		UnsignedByte_3_3_2,
		UnsignedByte_2_3_3_Rev,
		UnsignedShort_5_6_5,
		UnsignedShort_5_6_5_Rev,
		UnsignedShort_4_4_4_4,
		UnsignedShort_4_4_4_4_Rev,
		UnsignedShort_5_5_5_1,
		UnsignedShort_1_5_5_5_Rev,
		UnsignedInt_8_8_8_8,
		UnsignedInt_8_8_8_8_Rev,
		UnsignedInt_10_10_10_2,
		UnsignedInt_2_10_10_10_Rev,
		UnsignedInt_24_8,
		UnsignedInt_10F_11F_11F_Rev,
		UnsignedInt_5_9_9_9_Rev,
		Float32_UnsignedInt_24_8_Rev
	};

	enum class TextureInternalFormat : uint64_t
	{
		Undefined,

		// Common formats

		R_u8,
		R_i8,
		R_nu8,
		R_ni8,

		R_u16,
		R_i16,
		R_f16,
		R_nu16,
		R_ni16,

		R_u32,
		R_i32,
		R_f32,


		RG_u8,
		RG_i8,
		RG_nu8,
		RG_ni8,

		RG_u16,
		RG_i16,
		RG_f16,
		RG_nu16,
		RG_ni16,

		RG_u32,
		RG_i32,
		RG_f32,


		RGB_u8,
		RGB_i8,
		RGB_nu8,
		RGB_ni8,

		RGB_u16,
		RGB_i16,
		RGB_f16,
		RGB_nu16,
		RGB_ni16,

		RGB_u32,
		RGB_i32,
		RGB_f32,


		RGBA_u8,
		RGBA_i8,
		RGBA_nu8,
		RGBA_ni8,

		RGBA_u16,
		RGBA_i16,
		RGBA_f16,
		RGBA_nu16,
		RGBA_ni16,

		RGBA_u32,
		RGBA_i32,
		RGBA_f32,


		Depth_nu16,
		Depth_nu24,
		Depth_nu32,
		Depth_f32,

		Stencil_u1,
		Stencil_u4,
		Stencil_u8,
		Stencil_u16,

		Depth_nu24_Stencil_u8,
		Depth_f32_Stencil_u8,

		// Common space optimized formats

		RGB_nu4,
		RGB_nu5,
		RGB_nu10,
		RGB_nu12,

		R_nu3_G_nu3_B_nu2,
		R_nu5_G_nu6_B_nu5,
		R_f11_G_f11_B_f10,


		RGBA_nu2,
		RGBA_nu4,
		RGBA_nu12,

		RGB_nu5_A_nu1,
		RGB_u10_A_u2,
		RGB_nu10_A_nu2,

		// Special formats

		// RGB9_E5
		// sRGB formats
		// Compressed formats
	};

	class RawTexture
	{
		public:

			struct CreationParams
			{
				TextureTarget target = TextureTarget::Undefined;
				TextureInternalFormat internalFormat = TextureInternalFormat::Undefined;

				const Buffer* buffer = nullptr;
				uint32_t bufferOffset = 0;
				uint32_t bufferSize = 0;

				uint32_t width = 0;
				uint32_t height = 0;
				uint32_t depth = 0;

				uint32_t levels = 1;

				uint32_t samples = 0;
				bool fixedSampleLocations = false;
			};

			struct UpdateParams
			{
				const Buffer* buffer = nullptr;
				uint32_t bufferOffset = 0;

				const void* data = nullptr;

				TextureFormat dataFormat = TextureFormat::Undefined;
				TextureDataType dataType = TextureDataType::Undefined;

				uint32_t offsetX = 0;
				uint32_t offsetY = 0;
				uint32_t offsetZ = 0;

				uint32_t width = 0;
				uint32_t height = 0;
				uint32_t depth = 0;

				uint32_t level = 0;
			};

			RawTexture();
			RawTexture(const RawTexture& texture) = delete;
			RawTexture(RawTexture&& texture) = delete;

			RawTexture& operator=(const RawTexture& texture) = delete;
			RawTexture& operator=(RawTexture&& texture) = delete;

			void createNew(const CreationParams& params);
			void update(const UpdateParams& params);
			void destroy();

			// TODO: Update from framebuffer

			uint32_t getHandle() const;
			const CreationParams& getCreationParams() const;
			bool isValid() const;

			static void bind(const RawTexture& texture, TextureTarget target, uint32_t textureUnit = 0);
			static void unbind(TextureTarget target, uint32_t textureUnit = 0);

			~RawTexture();

		private:

			uint32_t _texture;
			CreationParams _creationParams;
	};

	class RenderBuffer
	{
		public:

			RenderBuffer() = delete;
			RenderBuffer(TextureInternalFormat internalFormat, const scp::u32vec2& size, uint32_t samples = 0);
			RenderBuffer(const RenderBuffer& renderBuffer) = delete;
			RenderBuffer(RenderBuffer&& renderBuffer) = delete;

			RenderBuffer& operator=(const RenderBuffer& renderBuffer) = delete;
			RenderBuffer& operator=(RenderBuffer&& renderBuffer) = delete;

			uint32_t getHandle() const;
			TextureInternalFormat getInternalFormat() const;
			const scp::u32vec2& getSize() const;

			~RenderBuffer();

		private:

			uint32_t _renderBuffer;
			TextureInternalFormat _internalFormat;
			scp::u32vec2 _size;
	};
}
