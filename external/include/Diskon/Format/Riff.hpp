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
		namespace riff
		{
			enum class ChunkType
			{
				StandardChunk,
				RiffChunk,
				ListChunk
			};

			struct ChunkHeader
			{
				ChunkType type;
				char id[4];
				uint32_t size;	// Result of chunk.computeInnerSize()
			};

			struct Chunk
			{
				ChunkType type;
				char id[4];
				std::vector<Chunk> subChunks;
				std::vector<uint8_t> data;

				uint32_t computeInnerSize() const;
				uint32_t computeTotalSize() const;
			};
		}

		class RiffIStream : public FormatIStream
		{
			public:

				RiffIStream();
				RiffIStream(const RiffIStream& stream) = delete;
				RiffIStream(RiffIStream&& stream) = delete;

				RiffIStream& operator=(const RiffIStream& stream) = delete;
				RiffIStream& operator=(RiffIStream&& stream) = delete;

				const FormatError& readFile(riff::File& file);
				const FormatError& readFileHeader(riff::FileHeader& header);
				const FormatError& readChunk(riff::Chunk& chunk);
				const FormatError& readChunkHeader(riff::ChunkHeader& header);
				template<typename TValue> const FormatError& readChunkData(TValue& value);
				template<typename TValue> const FormatError& readChunkData(TValue* values, uint32_t count);
				const FormatError& skipChunkContent();

				uint32_t computeRemainingSize() const;

				~RiffIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readChunk(riff::Chunk& chunk);
				const FormatError& _readChunkHeader(riff::ChunkHeader& header);
				const FormatError& _readChunkEnd();

				std::vector<std::array<uint32_t, 2>> _remainingSizes;
				bool _readingData;
		};

		class RiffOStream : public FormatOStream
		{
			public:

				RiffOStream();
				RiffOStream(const RiffOStream& stream) = delete;
				RiffOStream(RiffOStream&& stream) = delete;

				RiffOStream& operator=(const RiffOStream& stream) = delete;
				RiffOStream& operator=(RiffOStream&& stream) = delete;

				const FormatError& writeFile(const riff::File& file);
				const FormatError& writeFileHeader(const riff::FileHeader& header);
				const FormatError& writeChunk(const riff::Chunk& chunk);
				const FormatError& writeChunkHeader(const riff::ChunkHeader& header);
				template<typename TValue> const FormatError& writeChunkData(const TValue& value);
				template<typename TValue> const FormatError& writeChunkData(const TValue* values, uint32_t count);

				uint32_t computeRemainingSize() const;

				~RiffOStream() = default;

			private:

				void onDestinationRemoved() override final;

				const FormatError& _writeChunk(const riff::Chunk& chunk);
				const FormatError& _writeChunkHeader(const riff::ChunkHeader& header);
				const FormatError& _writeChunkEnd();

				std::vector<std::array<uint32_t, 2>> _remainingSizes;
				bool _writingData;
		};
	}
}
