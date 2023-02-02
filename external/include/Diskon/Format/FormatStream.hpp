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
		enum class FormatErrorCode
		{
			// Generic

			None,
			FileNotFound,
			FileOpenFailed,
			InvalidStream,
			NoStream,
			ExpectFailed,
			NotImplementedYet,

			// Deflate

			DeflateInvalidCompressionType,
			DeflateInvalidBlockLength,
			DeflateInvalidCodeLengths,
			DeflateInvalidCode,

			// Obj

			ObjInvalidEndLine,
			ObjInvalidNumber,
			ObjInvalidLine,

			// Png

			PngInvalidSignature,
			PngInvalidChunkLayout,
			PngInvalidChunkSize,
			PngInvalidChunkContent,
			PngInvalidChunkCRC,

			// Pnm

			PnmInvalidMagicNumber,
			PnmInvalidNumber,
			PnmMissingSpace,

			// Riff

			RiffInvalidChunkType,
			RiffInvalidChunkSize,
			RiffInvalidPadByte,

			// Wave

			WaveInvalidFormType,
			WaveInvalidFmtChunk,
			WaveInvalidFactChunk,

			// Xml

			XmlInvalidDeclaration,
			XmlInvalidDoctype,
			XmlInvalidProcessingInstruction,
			XmlInvalidElement,

			// Zlib

			ZlibInvalidFlagCheck,
			ZlibInvalidCompressionMethod,
			ZlibInvalidChecksum,
		};

		struct FormatError
		{
			FormatError();
			FormatError(FormatErrorCode code, const std::string& message);
			FormatError(const FormatError& error) = default;
			FormatError(FormatError&& error) = default;

			FormatError& operator=(const FormatError& error) = default;
			FormatError& operator=(FormatError&& error) = default;

			explicit operator bool() const;

			void clear();

			~FormatError() = default;

			FormatErrorCode errorCode;
			std::string errorMessage;
		};

		class FormatIStream
		{
			public:

				FormatIStream(const FormatIStream& stream) = delete;
				FormatIStream(FormatIStream&& stream) = delete;

				FormatIStream& operator=(const FormatIStream& stream) = delete;
				FormatIStream& operator=(FormatIStream&& stream) = delete;

				const FormatError& setSource(std::nullptr_t);
				const FormatError& setSource(const std::filesystem::path& path);
				const FormatError& setSource(std::istream& stream);

				const FormatError& getError() const;

				virtual ~FormatIStream();

			protected:

				FormatIStream(std::endian byteEndianness = std::endian::native);

				template<std::derived_from<FormatIStream> TSubStream> uint64_t addSubStream();
				void finishSubStreamTree();
				FormatIStream* getSubStream(uint64_t index);

				virtual void onSourceRemoved() = 0;

				std::streampos getPos();
				const FormatError& setPos(std::streampos pos);

				template<typename TValue> const FormatError& read(TValue& value);
				template<typename TValue> const FormatError& read(TValue* values, uint64_t count);
				const FormatError& expect(char token);
				const FormatError& expect(const char* token, uint64_t size);
				const FormatError& unget();
				const FormatError& unget(uint64_t count);

				FormatError* _error;

			private:

				std::istream* _stream;
				bool _streamOwned;

				std::endian _byteEndianness;

				bool _isSubStream;
				std::vector<FormatIStream*> _subStreams;
		};

		class FormatOStream
		{
			public:

				FormatOStream(const FormatOStream& stream) = delete;
				FormatOStream(FormatOStream&& stream) = delete;

				FormatOStream& operator=(const FormatOStream& stream) = delete;
				FormatOStream& operator=(FormatOStream&& stream) = delete;

				const FormatError& setDestination(std::nullptr_t);
				const FormatError& setDestination(const std::filesystem::path& path);
				const FormatError& setDestination(std::ostream& stream);

				const FormatError& getError() const;

				virtual ~FormatOStream();

			protected:

				FormatOStream(std::endian byteEndianness = std::endian::native);

				template<std::derived_from<FormatOStream> TSubStream> uint64_t addSubStream();
				void finishSubStreamTree();
				FormatOStream* getSubStream(uint64_t index);

				virtual void onDestinationRemoved() = 0;

				std::streampos getPos();
				const FormatError& setPos(std::streampos pos);

				template<typename TValue> const FormatError& write(const TValue& value);
				template<typename TValue> const FormatError& write(const TValue* values, uint64_t count);

				FormatError* _error;

			private:

				std::ostream* _stream;
				bool _streamOwned;

				std::endian _byteEndianness;

				bool _isSubStream;
				std::vector<FormatOStream*> _subStreams;
		};
	}
}
