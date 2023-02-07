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
		template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes> struct NextVertexBase { using Type = VertexBase<-1, -1, -1, TNextAttributes...>; };
		template<CVertexAttribute TAttribute> struct NextVertexBase<TAttribute> { using Type = VertexBase<-1, -1, -1, TAttribute>; };
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr uint32_t VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::getAttributeCount()
	{
		return sizeof...(TAttributes);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr spl::GlslType VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::getAttributeType(uint32_t i)
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

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr uint32_t VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::getAttributeOffsetInStructure(uint32_t i)
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

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr const void* VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::getAttribute(uint32_t i) const
	{
		return reinterpret_cast<const uint8_t*>(this) + getAttributeOffsetInStructure(i);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr void* VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::getAttribute(uint32_t i)
	{
		return reinterpret_cast<uint8_t*>(this) + getAttributeOffsetInStructure(i);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr void VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::setPosition(float x, float y, float z, float w)
	{
		_setVector<PositionIndex>(x, y, z, w);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr void VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::setNormal(float x, float y, float z, float w)
	{
		_setVector<NormalIndex>(x, y, z, w);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	constexpr void VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::setTexCoords(float x, float y, float z, float w)
	{
		_setVector<TexCoordsIndex>(x, y, z, w);
	}

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	template<uint32_t Index>
	constexpr void VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>::_setVector(float x, float y, float z, float w)
	{
		if constexpr (Index != -1)
		{
			void* attr = getAttribute(Index);

			if constexpr (std::same_as<AttributeType<Index>, float>)
			{
				float& attr = *reinterpret_cast<float*>(getAttribute(Index));
				attr = x;
			}
			else if constexpr (std::same_as<AttributeType<Index>, scp::f32vec2>)
			{
				scp::f32vec2& attr = *reinterpret_cast<scp::f32vec2*>(getAttribute(Index));
				attr.x = x;
				attr.y = y;
			}
			else if constexpr (std::same_as<AttributeType<Index>, scp::f32vec3>)
			{
				scp::f32vec3& attr = *reinterpret_cast<scp::f32vec3*>(getAttribute(Index));
				attr.x = x;
				attr.y = y;
				attr.z = z;
			}
			else if constexpr (std::same_as<AttributeType<Index>, scp::f32vec4>)
			{
				scp::f32vec4& attr = *reinterpret_cast<scp::f32vec4*>(getAttribute(Index));
				attr.x = x;
				attr.y = y;
				attr.z = z;
				attr.w = w;
			}
			else
			{
				assert(false);
			}
		}
	}


	constexpr DefaultVertex::DefaultVertex(const scp::f32vec3& pos, const scp::f32vec3& n, const scp::f32vec2& tex) :
		position(pos),
		normal(n),
		texCoords(tex)
	{
	}
}
