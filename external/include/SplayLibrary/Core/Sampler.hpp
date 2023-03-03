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
	enum class CompareFunc
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	enum class TextureCompareMode
	{
		None,
		RefToTexture
	};

	enum class TextureFiltering
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapNearest,
		NearestMipmapLinear,
		LinearMipmapLinear
	};

	enum class TextureWrapping
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
		MirrorClampToEdge
	};

	class Sampler
	{
		public:

			Sampler();
			Sampler(const Sampler& sampler);
			Sampler(Sampler&& sampler);

			Sampler& operator=(const Sampler& sampler);
			Sampler& operator=(Sampler&& sampler);

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

			~Sampler();

			static void bind(const Sampler* sampler, uint32_t textureUnit);
			static void bind(const Sampler* const* samplers, uint32_t firstUnit, uint32_t count);

		private:

			void _copyFrom(const Sampler& sampler);
			void _moveFrom(Sampler&& sampler);
			void _destroy();

			uint32_t _sampler;

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
