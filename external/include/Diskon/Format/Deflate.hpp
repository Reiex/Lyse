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
		namespace deflate
		{
			enum class CompressionType
			{
				NoCompression	= 0b00,
				FixedHuffman	= 0b01,
				DynamicHuffman	= 0b10
			};

			struct BlockHeader
			{
				bool isFinal;
				CompressionType compressionType;
				std::array<uint8_t, 288> litlenCodeLengths;
				std::array<uint8_t, 32> distCodeLengths;
			};

			struct Block
			{
				BlockHeader header;
				std::vector<uint8_t> data;
			};

			struct File
			{
				std::vector<Block> blocks;
			};
		}

		class DeflateIStream : public FormatIStream
		{
			public:

				DeflateIStream();
				DeflateIStream(const DeflateIStream& stream) = delete;
				DeflateIStream(DeflateIStream&& stream) = delete;

				DeflateIStream& operator=(const DeflateIStream& stream) = delete;
				DeflateIStream& operator=(DeflateIStream&& stream) = delete;

				const FormatError& readFile(deflate::File& file);
				const FormatError& readBlock(deflate::Block& block);
				const FormatError& readBlockHeader(deflate::BlockHeader& header);
				const FormatError& readBlockData(void* data, uint64_t size, uint64_t& sizeRead);
				const FormatError& readBlockEnd();

				~DeflateIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readNextByte(uint8_t& byte);
				const FormatError& _readSymbol(const HuffmanTree& tree, uint64_t& symbol);

				BitIStream* _bitStream;

				HuffmanTree _litlenTree;
				HuffmanTree _distTree;

				bool _hasCompression;
				uint16_t _remainingSize;

				bool _readingBlock;
				bool _readingLastBlock;
				bool _blockLastByteRead;
				bool _blockEndRead;

				std::vector<uint8_t> _lastBytes;
				uint32_t _lastByteIndex;
				std::array<uint16_t, 2> _repeater;
		};

		class DeflateOStream : public FormatOStream
		{
			public:

				DeflateOStream();
				DeflateOStream(const DeflateOStream& stream) = delete;
				DeflateOStream(DeflateOStream&& stream) = delete;

				DeflateOStream& operator=(const DeflateOStream& stream) = delete;
				DeflateOStream& operator=(DeflateOStream&& stream) = delete;

				const FormatError& writeFile(const deflate::File& file);
				const FormatError& writeBlock(const deflate::Block& block);
				const FormatError& writeBlockHeader(const deflate::BlockHeader& header, uint16_t size = 0);
				const FormatError& writeBlockData(const void* data, uint64_t size);
				const FormatError& writeBlockEnd();

				~DeflateOStream() = default;

			private:

				void onDestinationRemoved() override final;

				const FormatError& _writeSymbol(uint16_t symbol, const HuffmanTree& tree);

				BitOStream* _bitStream;

				HuffmanTree _litlenTree;
				HuffmanTree _distTree;

				bool _hasCompression;
				uint16_t _remainingSize;

				bool _writtingBlock;
				bool _writtingLastBlock;
				bool _blockEndWritten;

				std::vector<uint8_t> _lastBytes;
				uint64_t _lastByteIndex;
		};
	}
}
