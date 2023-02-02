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
	#pragma pack(push, 1)
	template<typename TComponent, uint8_t ComponentCount>
	class Pixel
	{
		static_assert(ComponentCount != 0);

		public:

			using ComponentType = TComponent;
			static constexpr uint8_t componentCount = ComponentCount;

			constexpr Pixel() = default;
			constexpr Pixel(const TComponent& value);
			constexpr Pixel(const std::initializer_list<TComponent>& components);
			constexpr Pixel(const Pixel<TComponent, ComponentCount>& pixel) = default;
			constexpr Pixel(Pixel<TComponent, ComponentCount>&& pixel) = default;
	
			constexpr Pixel<TComponent, ComponentCount>& operator=(const Pixel<TComponent, ComponentCount>& pixel) = default;
			constexpr Pixel<TComponent, ComponentCount>& operator=(Pixel<TComponent, ComponentCount>&& pixel) = default;
	
			constexpr TComponent& operator[](uint8_t i);
			constexpr const TComponent& operator[](uint8_t i) const;

			constexpr Pixel<TComponent, ComponentCount>& operator+=(const Pixel<TComponent, ComponentCount>& pixel);
			constexpr Pixel<TComponent, ComponentCount>& operator-=(const Pixel<TComponent, ComponentCount>& pixel);
			constexpr Pixel<TComponent, ComponentCount>& operator*=(float value);
			constexpr Pixel<TComponent, ComponentCount>& operator/=(float value);
	
			constexpr bool operator==(const Pixel<TComponent, ComponentCount>& pixel);
			constexpr bool operator!=(const Pixel<TComponent, ComponentCount>& pixel);
	
			template<typename T> constexpr void set(uint8_t i, const T& value);
			template<typename T> constexpr void get(uint8_t i, T& value) const;
	
			constexpr ~Pixel() = default;
	
		private:
	
			TComponent _components[ComponentCount];
	};
	#pragma pack(pop)


	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator+(const Pixel<TComponent, ComponentCount>& a, const Pixel<TComponent, ComponentCount>& b);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(Pixel<TComponent, ComponentCount>&& a, const Pixel<TComponent, ComponentCount>& b);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(const Pixel<TComponent, ComponentCount>& a, Pixel<TComponent, ComponentCount>&& b);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(Pixel<TComponent, ComponentCount>&& a, Pixel<TComponent, ComponentCount>&& b);

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator-(const Pixel<TComponent, ComponentCount>& a, const Pixel<TComponent, ComponentCount>& b);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator-(Pixel<TComponent, ComponentCount>&& a, const Pixel<TComponent, ComponentCount>& b);

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator*(const Pixel<TComponent, ComponentCount>& a, float x);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator*(Pixel<TComponent, ComponentCount>&& a, float x);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator*(float x, const Pixel<TComponent, ComponentCount>& a);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator*(float x, Pixel<TComponent, ComponentCount>&& a);

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator/(const Pixel<TComponent, ComponentCount>& a, float x);
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator/(Pixel<TComponent, ComponentCount>&& a, float x);


	using Pixel_gs_u8 = Pixel<uint8_t, 1>;
	using Pixel_gs_i8 = Pixel<int8_t, 1>;
	using Pixel_gs_u16 = Pixel<uint16_t, 1>;
	using Pixel_gs_i16 = Pixel<int16_t, 1>;
	using Pixel_gs_f32 = Pixel<float, 1>;
	using Pixel_rgb_u8 = Pixel<uint8_t, 3>;
	using Pixel_rgb_i8 = Pixel<int8_t, 3>;
	using Pixel_rgb_u16 = Pixel<uint16_t, 3>;
	using Pixel_rgb_i16 = Pixel<int16_t, 3>;
	using Pixel_rgb_f32 = Pixel<float, 3>;
	using Pixel_rgba_u8 = Pixel<uint8_t, 4>;
	using Pixel_rgba_i8 = Pixel<int8_t, 4>;
	using Pixel_rgba_u16 = Pixel<uint16_t, 4>;
	using Pixel_rgba_i16 = Pixel<int16_t, 4>;
	using Pixel_rgba_f32 = Pixel<float, 4>;
}
