///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreDecl.hpp>

namespace lys
{
	template<CVertexAttribute TAttribute>
	constexpr Vertex<TAttribute>::Vertex(TAttribute attrib) :
		_attrib(attrib)
	{
	}

	template<CVertexAttribute TAttribute>
	template<uint32_t Index>
	constexpr const Vertex<TAttribute>::AttributeType<Index>& Vertex<TAttribute>::getAttribute() const
	{
		static_assert(Index == 0);
		return _attrib;
	}

	template<CVertexAttribute TAttribute>
	template<uint32_t Index>
	constexpr Vertex<TAttribute>::AttributeType<Index>& Vertex<TAttribute>::getAttribute()
	{
		static_assert(Index == 0);
		return _attrib;
	}

	template<CVertexAttribute TAttribute>
	constexpr const void* Vertex<TAttribute>::getAttribute(uint32_t i) const
	{
		assert(i == 0);
		return &_attrib;
	}

	template<CVertexAttribute TAttribute>
	constexpr void* Vertex<TAttribute>::getAttribute(uint32_t i)
	{
		assert(i == 0);
		return &_attrib;
	}

	template<CVertexAttribute TAttribute>
	consteval uint32_t Vertex<TAttribute>::getAttributeCount()
	{
		return 1;
	}

	template<CVertexAttribute TAttribute>
	constexpr spl::GlslType Vertex<TAttribute>::getAttributeType(uint32_t i)
	{
		assert(i == 0);

		if constexpr (spl::CGlslScalarType<TAttribute>)
		{
			return spl::_spl::glslScalarTypeToGlslType<TAttribute>();
		}
		else if constexpr (spl::CGlslVecType<TAttribute>)
		{
			return spl::_spl::glslVecTypeToGlslType<TAttribute>();
		}
		else
		{
			assert(false);
			return spl::GlslType::Undefined;
		}
	}

	template<CVertexAttribute TAttribute>
	constexpr uint32_t Vertex<TAttribute>::getAttributeOffsetInStructure(uint32_t i)
	{
		assert(i == 0);
		return offsetof(Vertex<TAttribute>, _attrib);
	}


	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	constexpr Vertex<TAttribute, TNextAttributes...>::Vertex(TAttribute attrib, TNextAttributes... nextAttribs) :
		_attrib(attrib),
		_next(nextAttribs...)
	{
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	template<uint32_t Index>
	constexpr const Vertex<TAttribute, TNextAttributes...>::AttributeType<Index>& Vertex<TAttribute, TNextAttributes...>::getAttribute() const
	{
		static_assert(Index < 1 + sizeof...(TNextAttributes));
		
		if constexpr (Index == 0)
		{
			return _attrib;
		}
		else
		{
			return _next.getAttribute<Index - 1>();
		}
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	template<uint32_t Index>
	constexpr Vertex<TAttribute, TNextAttributes...>::AttributeType<Index>& Vertex<TAttribute, TNextAttributes...>::getAttribute()
	{
		static_assert(Index < 1 + sizeof...(TNextAttributes));

		if constexpr (Index == 0)
		{
			return _attrib;
		}
		else
		{
			return _next.getAttribute<Index - 1>();
		}
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	constexpr const void* Vertex<TAttribute, TNextAttributes...>::getAttribute(uint32_t i) const
	{
		assert(i < 1 + sizeof...(TNextAttributes));

		if (i == 0)
		{
			return &_attrib;
		}
		else
		{
			return _next.getAttribute(i - 1);
		}
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	constexpr void* Vertex<TAttribute, TNextAttributes...>::getAttribute(uint32_t i)
	{
		assert(i < 1 + sizeof...(TNextAttributes));

		if (i == 0)
		{
			return &_attrib;
		}
		else
		{
			return _next.getAttribute(i - 1);
		}
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	consteval uint32_t Vertex<TAttribute, TNextAttributes...>::getAttributeCount()
	{
		return 1 + sizeof...(TNextAttributes);
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	constexpr spl::GlslType Vertex<TAttribute, TNextAttributes...>::getAttributeType(uint32_t i)
	{
		assert(i < 1 + sizeof...(TNextAttributes));

		if (i == 0)
		{
			if constexpr (spl::CGlslScalarType<TAttribute>)
			{
				return spl::_spl::glslScalarTypeToGlslType<TAttribute>();
			}
			else if constexpr (spl::CGlslVecType<TAttribute>)
			{
				return spl::_spl::glslVecTypeToGlslType<TAttribute>();
			}
			else
			{
				assert(false);
				return spl::GlslType::Undefined;
			}
		}
		else
		{
			return Vertex<TNextAttributes...>::getAttributeType(i - 1);
		}
	}

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
	constexpr uint32_t Vertex<TAttribute, TNextAttributes...>::getAttributeOffsetInStructure(uint32_t i)
	{
		using This = Vertex<TAttribute, TNextAttributes...>;

		assert(i < 1 + sizeof...(TNextAttributes));

		if (i == 0)
		{
			return offsetof(This, _attrib);
		}
		else
		{
			return offsetof(This, _next) + Vertex<TNextAttributes...>::getAttributeOffsetInStructure(i - 1);
		}
	}
}
