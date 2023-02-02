///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Core/CoreTypes.hpp>

namespace dsk
{
	template<typename TValue>
	constexpr TValue byteswap(TValue x);

	template<std::unsigned_integral TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::unsigned_integral TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::unsigned_integral TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::signed_integral TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::signed_integral TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::signed_integral TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::floating_point TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::floating_point TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to);
	template<std::floating_point TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to);
}
