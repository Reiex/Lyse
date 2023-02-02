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
		namespace xml
		{
			struct Declaration
			{
				uint16_t versionMajor;
				uint16_t versionMinor;
				std::optional<std::string> encoding;
				std::optional<bool> standalone;
			};

			struct Doctype
			{
				std::string name;
				// TODO
			};

			struct ProcessingInstruction
			{
				std::string target;
				std::string instruction;
			};

			struct Prolog
			{
				std::optional<Declaration> declaration;
				std::optional<Doctype> doctype;
				std::vector<ProcessingInstruction> instructions;
			};


			struct ElementTag
			{
				std::string name;
				std::unordered_map<std::string, std::string> attributes;
				bool isEmpty;
			};

			struct Element
			{
				ElementTag tag;
				std::vector<ElementContent> contents;
			};

			namespace ContentType
			{
				enum Flags : uint8_t
				{
					EndOfElement			= 0,		///< Cannot be filtered away, exits the tag
					ChildTag				= 1 << 0,	///< Enters the child but do not exit it, so the next readContent is inside the child !
					Child					= 1 << 1,	///< On children, the filter does not apply. It applies only on the content directly read.
					CharData				= 1 << 2,	///< Any charData between two tags except comments
					ProcessingInstruction	= 1 << 3,
					// TODO: Add flag "NonEmptyCharData"

					Default					= 0b11111110
				};
			}

			struct ElementContent
			{
				ContentType::Flags type;

				ElementTag childTag;
				Element child;
				std::string charData;
				ProcessingInstruction instruction;
			};


			struct Ending
			{
				std::vector<ProcessingInstruction> instructions;
			};


			struct File
			{
				Prolog prolog;
				Element root;
				Ending ending;
			};
		}

		class XmlIStream : public FormatIStream
		{
			public:

				XmlIStream();
				XmlIStream(const XmlIStream& stream) = delete;
				XmlIStream(XmlIStream&& stream) = delete;

				XmlIStream& operator=(const XmlIStream& stream) = delete;
				XmlIStream& operator=(XmlIStream&& stream) = delete;

				const FormatError& readFile(xml::File& file);
				const FormatError& readProlog(xml::Prolog& prolog);
				const FormatError& readElementTag(xml::ElementTag& tag);
				const FormatError& readElementContent(xml::ElementContent& content, xml::ContentType::Flags filter = xml::ContentType::Default);
				const FormatError& readEnding(xml::Ending& ending);

				~XmlIStream() = default;

			private:

				void onSourceRemoved() override final;

				const FormatError& _readPIsSpacesAndComments(std::vector<xml::ProcessingInstruction>& instructions);
				const FormatError& _readElement(xml::Element& element);

				const FormatError& _readDeclaration(std::optional<xml::Declaration>& declaration);
				const FormatError& _readDoctype(std::optional<xml::Doctype>& doctype);
				const FormatError& _readProcessingInstruction(xml::ProcessingInstruction& instruction, bool& parsedSomething);

				const FormatError& _readEq();
				const FormatError& _readEncName(std::string& name);
				const FormatError& _readName(std::string& name);
				const FormatError& _readAttValue(std::string& value);

				const FormatError& _readSpaces(bool& parsedSomething);
				const FormatError& _readComment(bool& parsedSomething);

				std::vector<std::string> _tags;
				bool _prologRead;
				bool _treeRead;
		};

		class XmlOStream : public FormatOStream
		{
			public:

				XmlOStream();
				XmlOStream(const XmlOStream& stream) = delete;
				XmlOStream(XmlOStream&& stream) = delete;

				XmlOStream& operator=(const XmlOStream& stream) = delete;
				XmlOStream& operator=(XmlOStream&& stream) = delete;

				const FormatError& writeFile(const xml::File& file);
				const FormatError& writeProlog(const xml::Prolog& prolog);
				const FormatError& writeElementTag(const xml::ElementTag& tag);
				const FormatError& writeElementContent(const xml::ElementContent& content);
				const FormatError& writeEnding(const xml::Ending& ending);

				~XmlOStream() = default;

			private:

				void onDestinationRemoved() override final;

				const FormatError& _writeDeclaration(const xml::Declaration& declaration);
				const FormatError& _writeDoctype(const xml::Doctype& doctype);
				const FormatError& _writeProcessingInstruction(const xml::ProcessingInstruction& instruction);
				const FormatError& _writeElement(const xml::Element& element);

				const FormatError& _writeAttribute(const std::pair<const std::string, std::string>& attribute);

				std::vector<std::string> _tags;
				bool _prologWritten;
				bool _treeWritten;
		};
	}
}
