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
		namespace pnm
		{
			enum class Format
			{
				PlainPBM	= 1,
				PlainPGM	= 2,
				PlainPPM	= 3,
				RawPBM		= 4,
				RawPGM		= 5,
				RawPPM		= 6
			};

			struct Header
			{
				Format format;
				uint64_t width;
				uint64_t height;
				std::optional<uint16_t> maxSampleVal;
			};

			struct File
			{
				Header header;
				std::vector<uint16_t> samples;
			};
		}

		class PnmIStream : public FormatIStream
		{
			public:

				PnmIStream();
				PnmIStream(const PnmIStream& stream) = delete;
				PnmIStream(PnmIStream&& stream) = delete;

				PnmIStream& operator=(const PnmIStream& stream) = delete;
				PnmIStream& operator=(PnmIStream&& stream) = delete;

				const FormatError& readFile(pnm::File& file);
				const FormatError& readHeader(pnm::Header& header);
				const FormatError& readPixels(uint16_t* samples, uint64_t pixelCount);

				~PnmIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readHeaderSpaces();
				const FormatError& _readHeaderNumber(uint64_t& value);
				const FormatError& _readRasterSpaces(bool& parsedSomething);
				const FormatError& _readRasterNumber(uint16_t& value);
				const FormatError& _readRasterSample(uint16_t& sample);

				const FormatError& _readHeaderChar(char& value);

				BitIStream* _bitStream;

				pnm::Header _header;

				bool _headerRead;
				uint8_t _samplesPerPixel;
				uint64_t _remainingSamples;
		};

		class PnmOStream : public FormatOStream
		{
			public:

				PnmOStream();
				PnmOStream(const PnmOStream& stream) = delete;
				PnmOStream(PnmOStream&& stream) = delete;

				PnmOStream& operator=(const PnmOStream& stream) = delete;
				PnmOStream& operator=(PnmOStream&& stream) = delete;

				const FormatError& writeFile(const pnm::File& file);
				const FormatError& writeHeader(const pnm::Header& header);
				const FormatError& writePixels(const uint16_t* samples, uint64_t pixelCount);

				~PnmOStream() = default;

			private:

				void onDestinationRemoved() override final;

				const FormatError& writeNumber(uint64_t number);

				BitOStream* _bitStream;

				pnm::Header _header;

				bool _headerWritten;
				uint8_t _samplesPerPixel;
				uint64_t _remainingSamples;
		};
	}
}
