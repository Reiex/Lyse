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
	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>::Pixel(const TComponent& value)
	{
		std::fill_n(_components, ComponentCount, value);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>::Pixel(const std::initializer_list<TComponent>& components)
	{
		assert(components.size() == ComponentCount);
		std::copy_n(components.begin(), ComponentCount, _components);
	}
	
	template<typename TComponent, uint8_t ComponentCount>
	constexpr TComponent& Pixel<TComponent, ComponentCount>::operator[](uint8_t i)
	{
		assert(i < ComponentCount);
		return _components[i];
	}
	
	template<typename TComponent, uint8_t ComponentCount>
	constexpr const TComponent& Pixel<TComponent, ComponentCount>::operator[](uint8_t i) const
	{
		assert(i < ComponentCount);
		return _components[i];
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>& Pixel<TComponent, ComponentCount>::operator+=(const Pixel<TComponent, ComponentCount>& pixel)
	{
		std::transform(_components, _components + ComponentCount, pixel._components, _components, std::plus<TComponent>());
		return *this;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>& Pixel<TComponent, ComponentCount>::operator-=(const Pixel<TComponent, ComponentCount>& pixel)
	{
		std::transform(_components, _components + ComponentCount, pixel._components, _components, std::minus<TComponent>());
		return *this;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>& Pixel<TComponent, ComponentCount>::operator*=(float value)
	{
		std::transform(_components, _components + ComponentCount, _components, [&](const TComponent& x) { return x * value; });
		return *this;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>& Pixel<TComponent, ComponentCount>::operator/=(float value)
	{
		std::transform(_components, _components + ComponentCount, _components, [&](const TComponent& x) { return x / value; });
		return *this;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr bool Pixel<TComponent, ComponentCount>::operator==(const Pixel<TComponent, ComponentCount>& pixel)
	{
		return std::equal(_components, _components + ComponentCount, pixel._components);
	}
	
	template<typename TComponent, uint8_t ComponentCount>
	constexpr bool Pixel<TComponent, ComponentCount>::operator!=(const Pixel<TComponent, ComponentCount>& pixel)
	{
		return !std::equal(_components, _components + ComponentCount, pixel._components);
	}

	template<typename TComponent, uint8_t ComponentCount>
	template<typename T>
	constexpr void Pixel<TComponent, ComponentCount>::set(uint8_t i, const T& value)
	{
		assert(i < ComponentCount);
		dsk::convertNum<T, TComponent>(&value, _components + i);
	}

	template<typename TComponent, uint8_t ComponentCount>
	template<typename T>
	constexpr void Pixel<TComponent, ComponentCount>::get(uint8_t i, T& value) const
	{
		assert(i < ComponentCount);
		dsk::convertNum<TComponent, T>(_components + i, &value);
	}


	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator+(const Pixel<TComponent, ComponentCount>& a, const Pixel<TComponent, ComponentCount>& b)
	{
		Pixel<TComponent, ComponentCount> c(a);
		c += b;
		return c;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(Pixel<TComponent, ComponentCount>&& a, const Pixel<TComponent, ComponentCount>& b)
	{
		a += b;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(const Pixel<TComponent, ComponentCount>& a, Pixel<TComponent, ComponentCount>&& b)
	{
		b += a;
		return std::forward<Pixel<TComponent, ComponentCount>>(b);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator+(Pixel<TComponent, ComponentCount>&& a, Pixel<TComponent, ComponentCount>&& b)
	{
		a += b;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator-(const Pixel<TComponent, ComponentCount>& a, const Pixel<TComponent, ComponentCount>& b)
	{
		Pixel<TComponent, ComponentCount> c(a);
		c -= b;
		return c;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator-(Pixel<TComponent, ComponentCount>&& a, const Pixel<TComponent, ComponentCount>& b)
	{
		a -= b;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator*(const Pixel<TComponent, ComponentCount>& a, float x)
	{
		Pixel<TComponent, ComponentCount> b(a);
		b *= x;
		return b;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator*(Pixel<TComponent, ComponentCount>&& a, float x)
	{
		a *= x;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator*(float x, const Pixel<TComponent, ComponentCount>& a)
	{
		Pixel<TComponent, ComponentCount> b(a);
		b *= x;
		return b;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator*(float x, Pixel<TComponent, ComponentCount>&& a)
	{
		a *= x;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount> operator/(const Pixel<TComponent, ComponentCount>& a, float x)
	{
		Pixel<TComponent, ComponentCount> b(a);
		b /= x;
		return b;
	}

	template<typename TComponent, uint8_t ComponentCount>
	constexpr Pixel<TComponent, ComponentCount>&& operator/(Pixel<TComponent, ComponentCount>&& a, float x)
	{
		a /= x;
		return std::forward<Pixel<TComponent, ComponentCount>>(a);
	}
}
