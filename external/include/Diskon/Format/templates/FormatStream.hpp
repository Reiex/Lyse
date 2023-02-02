///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2022-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Diskon/Format/FormatDecl.hpp>

namespace dsk
{
	namespace fmt
	{
		template<std::derived_from<FormatIStream> TSubStream>
		uint64_t FormatIStream::addSubStream()
		{
			_subStreams.push_back(new TSubStream());
			_subStreams.back()->_isSubStream = true;

			return _subStreams.size() - 1;
		}

		template<typename TValue>
		const FormatError& FormatIStream::read(TValue& value)
		{
			FMT_BEGIN("FormatIStream::read(TValue& value)");

			FMT_READ(&value, 1);

			return *_error;
		}

		template<typename TValue>
		const FormatError& FormatIStream::read(TValue* values, uint64_t count)
		{
			FMT_BEGIN("FormatIStream::read(TValue* values, uint64_t count)");

			assert(values);
			assert(count != 0);

			// Read data

			const uint64_t size = sizeof(TValue) * count;
			FMT_VERIFY(_stream->read(reinterpret_cast<char*>(values), size), InvalidStream, "Error while reading " + std::to_string(size) + " bytes.");

			// Byteswap if necessary

			if constexpr (sizeof(TValue) > 1)
			{
				if (_byteEndianness != std::endian::native)
				{
					for (TValue* it = values, *itEnd = values + count; it != itEnd; ++it)
					{
						*it = byteswap(*it);
					}
				}
			}

			return *_error;
		}


		template<std::derived_from<FormatOStream> TSubStream>
		uint64_t FormatOStream::addSubStream()
		{
			_subStreams.push_back(new TSubStream());
			_subStreams.back()->_isSubStream = true;

			return _subStreams.size() - 1;
		}

		template<typename TValue>
		const FormatError& FormatOStream::write(const TValue& value)
		{
			FMT_BEGIN("FormatOStream::write(const TValue& value)");

			FMT_WRITE(&value, 1);

			return *_error;
		}

		template<typename TValue>
		const FormatError& FormatOStream::write(const TValue* values, uint64_t count)
		{
			FMT_BEGIN("FormatOStream::write(const TValue* values, uint64_t count)");

			assert(values);
			assert(count != 0);

			// Byteswap if necessary

			const uint64_t size = sizeof(TValue) * count;
			std::vector<TValue> memoryValues;
			if constexpr (sizeof(TValue) > 1)
			{
				if (_byteEndianness != std::endian::native)
				{
					memoryValues.resize(count);
					TValue* copy = memoryValues.data();

					const TValue* it = values;
					const TValue* const itEnd = values + count;
					for (; it != itEnd; ++it, ++copy)
					{
						*copy = byteswap(*it);
					}

					values = memoryValues.data();
				}
			}

			// Write data

			FMT_VERIFY(_stream->write(reinterpret_cast<const char*>(values), size), InvalidStream, "Error while writing " + std::to_string(size) + " bytes.");

			return *_error;
		}
	}
}
