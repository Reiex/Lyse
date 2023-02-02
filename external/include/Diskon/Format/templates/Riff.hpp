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
		template<typename TValue>
		const FormatError& RiffIStream::readChunkData(TValue& value)
		{
			FMT_BEGIN("RiffIStream::readChunkData(TValue& value)");

			FMT_CALL(readChunkData, &value, 1);

			return *_error;
		}

		template<typename TValue>
		const FormatError& RiffIStream::readChunkData(TValue* values, uint32_t count)
		{
			FMT_BEGIN("RiffIStream::readChunkData(TValue* values, uint32_t count)");

			const uint32_t size = count * sizeof(TValue);
			if (size == 0)
			{
				return *_error;
			}

			assert(!_remainingSizes.empty());
			assert(_readingData);

			FMT_VERIFY(_remainingSizes.back()[1] >= size, RiffInvalidChunkSize, "Invalid chunk size found or invalid size required.");
			_remainingSizes.back()[1] -= size;

			FMT_READ(values, count);

			if (_remainingSizes.back()[1] == 0)
			{
				FMT_CALL(_readChunkEnd);
			}

			return *_error;
		}

		template<typename TValue>
		const FormatError& RiffOStream::writeChunkData(const TValue& value)
		{
			FMT_BEGIN("RiffOStream::writeChunkData(const TValue& value)");

			FMT_CALL(writeChunkData, &value, 1);

			return *_error;
		}

		template<typename TValue>
		const FormatError& RiffOStream::writeChunkData(const TValue* values, uint32_t count)
		{
			FMT_BEGIN("RiffOStream::writeChunkData(const TValue* values, uint32_t count)");

			const uint32_t size = count * sizeof(TValue);
			if (size == 0)
			{
				return *_error;
			}

			assert(!_remainingSizes.empty());
			assert(_writingData);

			assert(_remainingSizes.back()[1] >= size);
			_remainingSizes.back()[1] -= size;

			FMT_WRITE(values, count);

			if (_remainingSizes.back()[1] == 0)
			{
				FMT_CALL(_writeChunkEnd);
			}

			return *_error;
		}
	}
}
