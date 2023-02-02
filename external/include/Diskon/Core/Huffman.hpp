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
	namespace huffman
	{
		enum class NodeType
		{
			Node,
			Leaf
		};

		class Node
		{
			public:

				Node(NodeType type = NodeType::Leaf);
				Node(const Node& node);
				Node(Node&& node);

				Node& operator=(const Node& node);
				Node& operator=(Node&& node);

				void setType(NodeType type);
				NodeType getType() const;

				Node* getParent();
				const Node* getParent() const;

				uint64_t getChildCount() const;
				
				Node* getChild(bool bit);
				const Node* getChild(bool bit) const;

				void setSymbol(uint64_t symbol);
				uint64_t getSymbol() const;

				~Node();

			private:

				void create(NodeType type, Node* parent);
				void copy(const Node& node);
				void move(Node&& node);
				void destroy();

				void incrementChildCount();
				void decrementChildCount();

				NodeType _type;
				Node* _parent;
				uint64_t _childCount;

				union
				{
					struct
					{
						Node* _bitOn;
						Node* _bitOff;
					};

					uint64_t _symbol;
				};
		};
	}
	
	class HuffmanTree
	{
		public:

			HuffmanTree() = default;
			HuffmanTree(const HuffmanTree& tree) = default;
			HuffmanTree(HuffmanTree&& tree) = default;

			HuffmanTree& operator=(const HuffmanTree& tree) = default;
			HuffmanTree& operator=(HuffmanTree&& tree) = default;

			void createFromNode(const huffman::Node& rootNode);
			void createFromCodeLengths(const uint64_t* symbols, const uint64_t* codeLengths, uint64_t symbolCount);
			// TODO: void createFromSymbolDistribution(const uint64_t* symbols, const float* symbolDist, uint64_t symbolCount);

			// Bit/Byte endianness are both LSB
			uint64_t readSymbol(const void* src, uint64_t bitOffset, uint64_t& bitsRead) const;
			void writeSymbol(void* dst, uint64_t symbol, uint64_t bitOffset, uint64_t& bitsWritten) const;

			uint64_t getCodeLength(uint64_t symbol) const;

			const huffman::Node& getRootNode() const;
			const std::unordered_map<uint64_t, uint64_t>& getSymbolToIndex() const;

			// TODO: void clear(); And use it in deflateStream

			~HuffmanTree() = default;

		private:

			huffman::Node _rootNode;
			std::unordered_map<uint64_t, uint64_t> _symbolToIndex;
	};
}
