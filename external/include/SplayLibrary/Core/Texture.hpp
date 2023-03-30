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
		Undefined,
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
		Undefined,
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

		// TODO: RGB9_E5
		// TODO: sRGB formats
		// TODO: Compressed formats
	};

	struct TextureCreationParams
	{
		TextureTarget target = TextureTarget::Undefined;
		TextureInternalFormat internalFormat = TextureInternalFormat::Undefined;

		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t depth = 0;

		uint32_t levels = 1;

		uint32_t samples = 1;
		bool fixedSampleLocations = false;

		const Buffer* buffer = nullptr;
		uintptr_t bufferSize = 0;
		uintptr_t bufferOffset = 0;
	};

	struct TextureUpdateParams
	{
		const void* data = nullptr;

		const Buffer* bufferData = nullptr;
		uintptr_t bufferOffset = 0;

		const Framebuffer* framebufferData = nullptr;
		uint32_t xFramebufferOffset = 0;
		uint32_t yFramebufferOffset = 0;

		TextureFormat dataFormat = TextureFormat::Undefined;
		TextureDataType dataType = TextureDataType::Undefined;

		uint32_t xOffset = 0;
		uint32_t yOffset = 0;
		uint32_t zOffset = 0;

		uint32_t width = 1;
		uint32_t height = 1;
		uint32_t depth = 1;

		uint32_t level = 0;
	};

	class Texture : public FramebufferAttachable
	{
		public:

			Texture();
			Texture(const TextureCreationParams& params);
			Texture(const Texture& texture) = delete;
			Texture(Texture&& texture) = delete;

			Texture& operator=(const Texture& texture) = delete;
			Texture& operator=(Texture&& texture) = delete;


			void createNew(const TextureCreationParams& params);
			void update(const TextureUpdateParams& params);
			// TODO: glClearTexSubImage
			// TODO: glInvalidateTexSubImage
			// TODO: Texture views (new class that Texture inherits from ?)
			void destroy();


			void setBorderColor(float r, float g, float b, float a);
			void setCompareMode(TextureCompareMode compareMode);
			void setCompareFunc(CompareFunc compareFunc);
			void setMinLod(float lod);
			void setMaxLod(float lod);
			void setLodBias(float bias);
			void setMinFiltering(TextureFiltering filtering);
			void setMagFiltering(TextureFiltering filtering);
			void setMaxAnisotropy(float maxAnisotropy);
			void setWrappingS(TextureWrapping wrap);
			void setWrappingT(TextureWrapping wrap);
			void setWrappingR(TextureWrapping wrap);


			bool isValid() const;
			uint32_t getHandle() const;
			const TextureCreationParams& getCreationParams() const;

			const scp::f32vec4& getBorderColor() const;
			TextureCompareMode getCompareMode() const;
			CompareFunc getCompareFunc() const;
			float getMinLod() const;
			float getMaxLod() const;
			float getLodBias() const;
			TextureFiltering getMinFiltering() const;
			TextureFiltering getMagFiltering() const;
			float getMaxAnisotropy() const;
			TextureWrapping getWrappingS() const;
			TextureWrapping getWrappingT() const;
			TextureWrapping getWrappingR() const;


			virtual ~Texture() override;


			static void bind(const Texture* texture, uint32_t textureUnit);
			static void bind(const Texture* const* textures, uint32_t firstUnit, uint32_t count);

		protected:

			void _newTextureParameters();

			struct ImageLoadInfos
			{
				TextureFormat format;
				TextureDataType dataType;

				void* data;
				uint32_t width;
				uint32_t height;

				void* image;
				void (*imageFree)(void*);
			};

			template<TextureInternalFormat InternalFormat> static void _loadImageFromFile(const std::filesystem::path& path, ImageLoadInfos& infos);
			static void _loadImageFromFile(const std::filesystem::path& path, TextureInternalFormat internalFormat, ImageLoadInfos& infos);

			uint32_t _texture;
			TextureCreationParams _params;

			// TODO: depth stencil texture mode, base level, max level, swizzle

			scp::f32vec4 _borderColor;
			TextureCompareMode _compareMode;
			CompareFunc _compareFunc;
			float _minLod;
			float _maxLod;
			float _lodBias;
			TextureFiltering _minFilter;
			TextureFiltering _magFilter;
			float _maxAnisotropy;
			TextureWrapping _sWrap;
			TextureWrapping _tWrap;
			TextureWrapping _rWrap;
	};
}
