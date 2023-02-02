///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Format/FormatTypes.hpp>

namespace dsk
{
	namespace fmt
	{
		class BitIStream : public FormatIStream
		{
			public:

				BitIStream();
				BitIStream(const BitIStream& stream) = delete;
				BitIStream(BitIStream&& stream) = delete;

				BitIStream& operator=(const BitIStream& stream) = delete;
				BitIStream& operator=(BitIStream&& stream) = delete;

				void setBitEndianness(std::endian bitEndianness);
				std::endian getBitEndianness() const;

				const FormatError& extractBytes(uint64_t count);
				const FormatError& readBits(uint8_t* data, uint64_t bitCount);
				const FormatError& discardBits(uint64_t bitCount);
				const FormatError& discardTrailingBits();

				const FormatError& ungetBits(uint64_t bitCount);

				uint64_t getBitsExtracted() const;

				~BitIStream() = default;

			private:

				void onSourceRemoved() override final;

				std::endian _bitEndianness;
				std::deque<uint8_t> _bytesExtracted;
				uint64_t _bitsExtracted;
		};

		class BitOStream : public FormatOStream
		{
			public:

				BitOStream();
				BitOStream(const BitOStream& stream) = delete;
				BitOStream(BitOStream&& stream) = delete;

				BitOStream& operator=(const BitOStream& stream) = delete;
				BitOStream& operator=(BitOStream&& stream) = delete;

				void setBitEndianness(std::endian bitEndianness);
				std::endian getBitEndianness() const;

				const FormatError& writeBits(const uint8_t* data, uint64_t bitCount);
				const FormatError& flush();

				~BitOStream() = default;

			private:

				void onDestinationRemoved() override final;

				std::endian _bitEndianness;
				uint8_t _byteWritten;
				uint8_t _bitsWritten;
		};
	}
}
