///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cctype>
#include <concepts>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace dsk
{
	template<uint8_t ExpSize, uint8_t MantissaSize> class Float;

	namespace _dsk
	{
		constexpr uint8_t baseTypeIndex(uint8_t n) { return n <= 8 ? 0 : n <= 16 ? 1 : n <= 32 ? 2 : 3; }
		template<uint8_t BitSize> using UnsignedIntBase = typename std::tuple_element<_dsk::baseTypeIndex(BitSize), std::tuple<uint8_t, uint16_t, uint32_t, uint64_t>>::type;
		template<uint8_t BitSize> using SignedIntBase = typename std::tuple_element<_dsk::baseTypeIndex(BitSize), std::tuple<int8_t, int16_t, int32_t, int64_t>>::type;
		template<uint8_t BitSize, typename TInt> class BaseInt;
	}
	template<uint8_t BitSize> using UnsignedInt = _dsk::BaseInt<BitSize, _dsk::UnsignedIntBase<BitSize>>;
	template<uint8_t BitSize> using SignedInt = _dsk::BaseInt<BitSize, _dsk::SignedIntBase<BitSize>>;

	template<typename TInt> class UnsignedIntSat;
	template<typename TInt> class SignedIntSat;


	namespace huffman
	{
		enum class NodeType;
		class Node;
	}
	class HuffmanTree;
}

using float16_t = dsk::Float<5, 10>;
using float32_t = float;
using float64_t = double;

using uint24_t = dsk::UnsignedInt<24>;
using uint40_t = dsk::UnsignedInt<40>;
using uint48_t = dsk::UnsignedInt<48>;
using uint56_t = dsk::UnsignedInt<56>;

using int24_t = dsk::SignedInt<24>;
using int40_t = dsk::SignedInt<40>;
using int48_t = dsk::SignedInt<48>;
using int56_t = dsk::SignedInt<56>;

using uint_sat8_t = dsk::UnsignedIntSat<uint8_t>;
using uint_sat16_t = dsk::UnsignedIntSat<uint16_t>;
using uint_sat24_t = dsk::UnsignedIntSat<uint24_t>;
using uint_sat32_t = dsk::UnsignedIntSat<uint32_t>;
using uint_sat40_t = dsk::UnsignedIntSat<uint40_t>;
using uint_sat48_t = dsk::UnsignedIntSat<uint48_t>;
using uint_sat56_t = dsk::UnsignedIntSat<uint56_t>;
using uint_sat64_t = dsk::UnsignedIntSat<uint64_t>;

using int_sat8_t = dsk::SignedIntSat<int8_t>;
using int_sat16_t = dsk::SignedIntSat<int16_t>;
using int_sat24_t = dsk::SignedIntSat<int24_t>;
using int_sat32_t = dsk::SignedIntSat<int32_t>;
using int_sat40_t = dsk::SignedIntSat<int40_t>;
using int_sat48_t = dsk::SignedIntSat<int48_t>;
using int_sat56_t = dsk::SignedIntSat<int56_t>;
using int_sat64_t = dsk::SignedIntSat<int64_t>;
