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
	namespace cksm
	{
		// CRC

		namespace _dsk
		{
			template<typename TValue, TValue RevPoly>
			class Crc
			{
				public:

					constexpr Crc();
					constexpr TValue operator()(const void* src, uint64_t size, TValue crc = 0) const;

				private:

					TValue _table[256];
			};
		}

		// Fletcher/Adler

		namespace _dsk
		{
			template<typename TValue, typename THalf, TValue Modulus>
			struct Fletcher
			{
				constexpr Fletcher() = default;
				constexpr TValue operator()(const void* src, uint64_t size, TValue initialValue = 1) const;
			};
		}
	}
}
