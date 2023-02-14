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
	namespace _lys
	{
		template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes>
		struct NextVertexBase
		{
			using Type = VertexBase<NoIndices, TNextAttributes...>;
		};

		template<CVertexAttribute TAttribute> struct NextVertexBase<TAttribute>
		{
			using Type = VertexBase<NoIndices, TAttribute>;
		};
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr uint32_t VertexBase<SpecialIndices, TAttributes...>::getAttributeCount()
	{
		return sizeof...(TAttributes);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr spl::GlslType VertexBase<SpecialIndices, TAttributes...>::getAttributeType(uint32_t i)
	{
		assert(i < sizeof...(TAttributes));

		if (i == 0)
		{
			using TAttribute = std::tuple_element<0, std::tuple<TAttributes...>>::type;

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
			return _lys::NextVertexBase<TAttributes...>::Type::getAttributeType(i - 1);
		}
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr uint32_t VertexBase<SpecialIndices, TAttributes...>::getAttributeOffsetInStructure(uint32_t i)
	{
		assert(i < sizeof...(TAttributes));

		if (i == 0)
		{
			return 0;
		}
		else
		{
			using TAttribute = std::tuple_element<0, std::tuple<TAttributes...>>::type;
			return sizeof(TAttribute) + _lys::NextVertexBase<TAttributes...>::Type::getAttributeOffsetInStructure(i - 1);
		}
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr const void* VertexBase<SpecialIndices, TAttributes...>::getAttribute(uint32_t i) const
	{
		return reinterpret_cast<const uint8_t*>(this) + getAttributeOffsetInStructure(i);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void* VertexBase<SpecialIndices, TAttributes...>::getAttribute(uint32_t i)
	{
		return reinterpret_cast<uint8_t*>(this) + getAttributeOffsetInStructure(i);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::setPosition(float x, float y, float z, float w)
	{
		_setVector<SpecialIndices.position>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::setNormal(float x, float y, float z, float w)
	{
		_setVector<SpecialIndices.normal>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::setTangent(float x, float y, float z, float w)
	{
		_setVector<SpecialIndices.tangent>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::setTexCoords(float x, float y, float z, float w)
	{
		_setVector<SpecialIndices.texCoords>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::getPosition(float& x, float& y, float& z, float& w) const
	{
		_getVector<SpecialIndices.position>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::getNormal(float& x, float& y, float& z, float& w) const
	{
		_getVector<SpecialIndices.normal>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::getTangent(float& x, float& y, float& z, float& w) const
	{
		_getVector<SpecialIndices.tangent>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::getTexCoords(float& x, float& y, float& z, float& w) const
	{
		_getVector<SpecialIndices.texCoords>(x, y, z, w);
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	template<uint32_t Index>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::_setVector(float x, float y, float z, float w)
	{
		if constexpr (Index != UINT32_MAX)
		{
			using TAttr = AttributeType<Index>;
			static_assert(scp::CVec<TAttr> && TAttr::size == 4);

			TAttr& attr = *reinterpret_cast<TAttr*>(getAttribute(Index));
			attr.x = x;
			attr.y = y;
			attr.z = z;
			attr.w = w;
		}
	}

	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	template<uint32_t Index>
	constexpr void VertexBase<SpecialIndices, TAttributes...>::_getVector(float& x, float& y, float& z, float& w) const
	{
		if constexpr (Index != UINT32_MAX)
		{
			using TAttr = AttributeType<Index>;
			static_assert(scp::CVec<TAttr> && TAttr::size == 4);

			const TAttr& attr = *reinterpret_cast<const TAttr*>(getAttribute(Index));
			x = attr.x;
			y = attr.y;
			z = attr.z;
			w = attr.w;
		}
		else
		{
			x = 0.f;
			y = 0.f;
			z = 0.f;
			w = 0.f;
		}
	}


	constexpr VertexDefaultMesh::VertexDefaultMesh() :
		position(0.f, 0.f, 0.f, 1.f),
		normal(0.f, 0.f, 0.f, 0.f),
		tangent(0.f, 0.f, 0.f, 0.f),
		texCoords(0.f, 0.f, 0.f, 0.f)
	{
	}

	constexpr VertexDefaultMesh::VertexDefaultMesh(const scp::f32vec4& p, const scp::f32vec4& n, const scp::f32vec4& t, const scp::f32vec4& tc) :
		position(p),
		normal(n),
		tangent(t),
		texCoords(tc)
	{
	}
}
