///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreTypes.hpp>

namespace lys
{
	template<CVertexAttribute TAttribute>
	class Vertex<TAttribute>
	{
		public:

			using IsVertex = bool;
			template<uint32_t Index> using AttributeType = TAttribute;

			constexpr Vertex() = default;
			constexpr Vertex(TAttribute attrib);
			constexpr Vertex(const Vertex<TAttribute>& vertex) = default;
			constexpr Vertex(Vertex<TAttribute>&& vertex) = default;

			Vertex<TAttribute>& operator=(const Vertex<TAttribute>& vertex) = default;
			Vertex<TAttribute>& operator=(Vertex<TAttribute>&& vertex) = default;

			template<uint32_t Index> constexpr const AttributeType<Index>& getAttribute() const;
			template<uint32_t Index> constexpr AttributeType<Index>& getAttribute();
			constexpr const void* getAttribute(uint32_t i) const;
			constexpr void* getAttribute(uint32_t i);

			constexpr ~Vertex() = default;

			static consteval uint32_t getAttributeCount();
			static constexpr spl::GlslType getAttributeType(uint32_t i);
			static constexpr uint32_t getAttributeOffsetInStructure(uint32_t i);

		private:

			TAttribute _attrib;
	};

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	class Vertex
	{
		public:

			using IsVertex = bool;
			template<uint32_t Index> using AttributeType = std::tuple_element<Index, std::tuple<TAttribute, TNextAttributes...>>::type;

			constexpr Vertex() = default;
			constexpr Vertex(TAttribute attrib, TNextAttributes... nextAttribs);
			constexpr Vertex(const Vertex<TAttribute, TNextAttributes...>& vertex) = default;
			constexpr Vertex(Vertex<TAttribute, TNextAttributes...>&& vertex) = default;

			Vertex<TAttribute, TNextAttributes...>& operator=(const Vertex<TAttribute, TNextAttributes...>& vertex) = default;
			Vertex<TAttribute, TNextAttributes...>& operator=(Vertex<TAttribute, TNextAttributes...>&& vertex) = default;

			template<uint32_t Index> constexpr const AttributeType<Index>& getAttribute() const;
			template<uint32_t Index> constexpr AttributeType<Index>& getAttribute();
			constexpr const void* getAttribute(uint32_t i) const;
			constexpr void* getAttribute(uint32_t i);

			constexpr ~Vertex() = default;

			static consteval uint32_t getAttributeCount();
			static constexpr spl::GlslType getAttributeType(uint32_t i);
			static constexpr uint32_t getAttributeOffsetInStructure(uint32_t i);

		private:

			TAttribute _attrib;
			Vertex<TNextAttributes...> _next;
	};
}
