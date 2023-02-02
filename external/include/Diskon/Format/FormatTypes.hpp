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
	namespace fmt
	{
		struct FormatError;
		class FormatIStream;
		class FormatOStream;

		class BitIStream;
		class BitOStream;

		namespace deflate
		{
			enum class CompressionType;
			struct BlockHeader;
			struct Block;
			struct File;
		}
		class DeflateIStream;
		class DeflateOStream;

		namespace zlib
		{
			enum class CompressionMethod : uint8_t;
			enum class CompressionLevel : uint8_t;
			struct Header;
			struct File;
		}
		class ZlibIStream;
		class ZlibOStream;

		namespace png
		{
			enum class ColorType : uint8_t;
			enum class CompressionMethod : uint8_t;
			enum class FilterMethod : uint8_t;
			enum class InterlaceMethod : uint8_t;
			struct ImageStructure;
			struct Chromaticity;
			struct ICCProfile;
			struct SignificantBits;
			enum class SRGBIntent : uint8_t;
			struct BackgroundColor;
			union TransparencyMask;
			enum class PixelDimensionUnit : uint8_t;
			struct PhysicalPixelDimensions;
			struct PaletteEntry;
			struct Palette;
			struct LastModification;
			struct TextualData;
			struct Header;
			struct Ending;
			template<typename TSample> struct File;
		}
		class PngIStream;
		class PngOStream;

		namespace riff
		{
			enum class ChunkType;
			struct ChunkHeader;
			struct Chunk;
			using FileHeader = ChunkHeader;
			using File = Chunk;
		}
		class RiffIStream;
		class RiffOStream;

		namespace wave
		{
			enum class Format : uint16_t;
			struct Metadata;
			struct Header;
			template<typename TSample> struct File;
		}
		class WaveIStream;
		class WaveOStream;

		namespace xml
		{
			struct Declaration;
			struct Doctype;
			struct ProcessingInstruction;
			struct Prolog;
			struct ElementTag;
			struct Element;
			namespace ContentType { enum Flags : uint8_t; }
			struct ElementContent;
			struct Ending;
			struct File;
		}
		class XmlIStream;
		class XmlOStream;

		namespace pnm
		{
			enum class Format;
			struct Header;
			struct File;
		}
		class PnmIStream;
		class PnmOStream;

		namespace obj
		{

		}
		class ObjIStream;
		class ObjOStream;
	}
}


// Verify macros

#define FMT_VERIFY(cond, code, message)						\
do {														\
	if (!(cond))											\
	{														\
		_error->errorCode = FormatErrorCode::code;			\
		_error->errorMessage = funcName + ": " + message;	\
		return *_error;										\
	}														\
} while (0)


#define FMT_CALL(call, ...)												\
do {																	\
	if (!call(__VA_ARGS__))												\
	{																	\
		_error->errorMessage = funcName + "\n" + _error->errorMessage;	\
		return *_error;													\
	}																	\
} while (0)


#define FMT_SUB_CALL(streamName, call, ...)								\
do {																	\
	if (!streamName->call(__VA_ARGS__))									\
	{																	\
		_error->errorMessage = funcName + "\n" + _error->errorMessage;	\
		return *_error;													\
	}																	\
} while (0)


#define FMT_READ(...)		\
FMT_CALL(read, __VA_ARGS__)


#define FMT_WRITE(...)			\
FMT_CALL(write, __VA_ARGS__)


// Function begining macros

#define FMT_BEGIN(name)						\
assert(*_error);							\
static const std::string funcName = name


// Sub-stream macros

#define FMT_CREATE_SUBSTREAM(streamName, streamType)						\
uint64_t index##streamName = addSubStream<streamType>();					\
streamName = dynamic_cast<streamType*>(getSubStream(index##streamName))


// FourCC constexpr const to uint32_t

namespace dsk
{
	namespace fmt
	{
		constexpr uint32_t fourcc(const char* id)
		{
			return id[0] << 24 | id[1] << 16 | id[2] << 8 | id[3];
		}
	}
}
