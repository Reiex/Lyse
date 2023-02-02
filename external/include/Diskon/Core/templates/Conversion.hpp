///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Core/CoreDecl.hpp>

namespace dsk
{
	template<typename TValue>
	constexpr TValue byteswap(TValue x)
	{
		if constexpr (std::is_integral_v<TValue>)
		{
			return std::byteswap(x);
		}
		else
		{
			uint8_t* it = reinterpret_cast<uint8_t*>(&x);
			uint8_t* itRev = it + sizeof(TValue) - 1;
			for (; it < itRev; ++it, --itRev)
			{
				std::swap(*it, *itRev);
			}

			return x;
		}
	}

	template<std::unsigned_integral TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		if constexpr (sizeof(TFrom) > sizeof(TTo))
		{
			constexpr uint8_t shift = (sizeof(TFrom) - sizeof(TTo)) * 8;
			rTo = rFrom >> shift;
		}
		else if constexpr (sizeof(TFrom) < sizeof(TTo))
		{
			constexpr uint8_t shiftInit = ((sizeof(TTo) - sizeof(TFrom)) % sizeof(TFrom)) * 8;
			constexpr uint8_t shift = sizeof(TFrom) * 8;
			constexpr uint8_t iter = (sizeof(TTo) - sizeof(TFrom)) / sizeof(TFrom);

			TTo rCastedFrom = static_cast<TTo>(rFrom);

			rTo = rCastedFrom << shiftInit;
			for (uint8_t i = 0; i < iter; ++i)
			{
				rTo = (rTo << shift) | rCastedFrom;
			}
		}
		else
		{
			rTo = static_cast<TTo>(rFrom);
		}
	}

	template<std::unsigned_integral TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		if constexpr (sizeof(TFrom) > sizeof(TTo))
		{
			constexpr uint8_t shift = (sizeof(TFrom) - sizeof(TTo)) * 8;
			constexpr TFrom offset = std::numeric_limits<TFrom>::max() / 2;
			rTo = static_cast<TTo>((rFrom + offset) >> shift);
		}
		else if constexpr (sizeof(TFrom) < sizeof(TTo))
		{
			rTo = std::numeric_limits<TTo>::lowest() + static_cast<TTo>(rFrom);
		}
		else
		{
			constexpr TFrom offset = std::numeric_limits<TFrom>::max() / 2;
			rTo = static_cast<TTo>(rFrom + offset);
		}
	}

	template<std::unsigned_integral TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		rTo = ((static_cast<TTo>(rFrom) * 2) / std::numeric_limits<TFrom>::max()) - 1;
	}

	template<std::signed_integral TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);
		
		constexpr TTo offset = std::numeric_limits<TTo>::max() / 2;

		if constexpr (sizeof(TFrom) > sizeof(TTo))
		{
			constexpr uint8_t shift = (sizeof(TFrom) - sizeof(TTo)) * 8;
			rTo = static_cast<TTo>(rFrom >> shift) + offset;
		}
		else if constexpr (sizeof(TFrom) < sizeof(TTo))
		{
			constexpr TTo smallOffset = (TTo(1) << (sizeof(TFrom) - 1)) - 1;
			constexpr uint8_t shiftInit = ((sizeof(TTo) - sizeof(TFrom)) % sizeof(TFrom)) * 8;
			constexpr uint8_t shift = sizeof(TFrom) * 8;
			constexpr uint8_t iter = (sizeof(TTo) - sizeof(TFrom)) / sizeof(TFrom);

			TTo rCastedFrom = static_cast<TTo>(rFrom) + smallOffset;

			rTo = rCastedFrom << shiftInit;
			for (uint8_t i = 0; i < iter; ++i)
			{
				rTo = (rTo << shift) | rCastedFrom;
			}
		}
		else
		{
			rTo = static_cast<TTo>(rFrom) + offset;
		}
	}

	template<std::signed_integral TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		if constexpr (sizeof(TFrom) > sizeof(TTo))
		{
			constexpr uint8_t shift = (sizeof(TFrom) - sizeof(TTo)) * 8;
			rTo = rFrom >> shift;
		}
		else if constexpr (sizeof(TFrom) < sizeof(TTo))
		{
			// TODO: Enhance that.
			constexpr uint8_t shift = (sizeof(TTo) - sizeof(TFrom)) * 8;
			rTo = rFrom << shift;
		}
		else
		{
			rTo = static_cast<TTo>(rFrom);
		}
	}

	template<std::signed_integral TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		if (rFrom >= 0)
		{
			rTo = static_cast<TTo>(rFrom) / std::numeric_limits<TFrom>::max();
		}
		else
		{
			rTo = -static_cast<TTo>(rFrom) / std::numeric_limits<TFrom>::lowest();
		}
	}

	template<std::floating_point TFrom, std::unsigned_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		rTo = static_cast<TTo>(((rFrom + 1) / 2) * std::numeric_limits<TTo>::max());
	}

	template<std::floating_point TFrom, std::signed_integral TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		if (rFrom >= 0)
		{
			rTo = static_cast<TTo>(rFrom * std::numeric_limits<TTo>::max());
		}
		else
		{
			rTo = static_cast<TTo>(-(rFrom * std::numeric_limits<TTo>::lowest()));
		}
	}

	template<std::floating_point TFrom, std::floating_point TTo>
	constexpr void convertNum(const void* from, void* to)
	{
		const TFrom& rFrom = *reinterpret_cast<const TFrom*>(from);
		TTo& rTo = *reinterpret_cast<TTo*>(to);

		rTo = static_cast<TTo>(rFrom);
	}
}
