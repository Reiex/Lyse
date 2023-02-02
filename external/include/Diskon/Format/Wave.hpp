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
		namespace wave
		{
			enum class Format : uint16_t
			{
				PCM			= 0x0001,
				Float		= 0x0003,
				// ALaw			= 0x0006,
				// MuLaw		= 0x0007,
				// Extensible	= 0xFFFE
			};

			struct Metadata
			{
				Format format;
				uint16_t channelCount;
				uint32_t frequency;
				uint16_t bitsPerSample;
			};

			struct Header
			{
				Metadata metadata;
				uint32_t blockCount;
			};

			template<typename TSample>
			struct File
			{
				Metadata metadata;
				std::vector<TSample> samples;
			};
		}

		class WaveIStream : public FormatIStream
		{
			public:

				WaveIStream();
				WaveIStream(const WaveIStream& stream) = delete;
				WaveIStream(WaveIStream&& stream) = delete;

				WaveIStream& operator=(const WaveIStream& stream) = delete;
				WaveIStream& operator=(WaveIStream&& stream) = delete;


				template<typename TSample> const FormatError& readFile(wave::File<TSample>& file);
				const FormatError& readHeader(wave::Header& header);
				template<typename TSample> const FormatError& readSampleBlocks(TSample* samples, uint32_t blockCount);
				const FormatError& skipBlocks(uint32_t blockCount);
				const FormatError& readEndFile();

				~WaveIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readFormatChunk(const riff::ChunkHeader& chunkHeader);
				// TODO: Split _readFormatChunk into sub functions for each format

				const FormatError& _readFactChunk(const riff::ChunkHeader& chunkHeader);
				// TODO: Read other optionnal chunks

				template<typename TRaw, typename TSample> const FormatError& _readRawSampleBlocks(TSample* samples, uint32_t blockCount);

				RiffIStream* _riffStream;
				bool _headerRead;
				wave::Header _header;
				uint32_t _remainingBlocks;
		};

		class WaveOStream : public FormatOStream
		{
			public:

				WaveOStream();
				WaveOStream(const WaveOStream& stream) = delete;
				WaveOStream(WaveOStream&& stream) = delete;

				WaveOStream& operator=(const WaveOStream& stream) = delete;
				WaveOStream& operator=(WaveOStream&& stream) = delete;


				template<typename TSample> const FormatError& writeFile(const wave::File<TSample>& file);
				const FormatError& writeHeader(const wave::Header& header);
				template<typename TSample> const FormatError& writeSampleBlocks(const TSample* samples, uint32_t blockCount);

				~WaveOStream() = default;

			private:

				void onDestinationRemoved() override final;

				template<typename TRaw, typename TSample> const FormatError& _writeRawSampleBlocks(const TSample* samples, uint32_t blockCount);

				RiffOStream* _riffStream;
				wave::Header _header;
				uint32_t _remainingBlocks;
		};
	}
}
