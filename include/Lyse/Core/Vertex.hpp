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
	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes>
	class VertexBase
	{
		static_assert(PositionIndex == -1 || PositionIndex < sizeof...(TAttributes));
		static_assert(NormalIndex == -1 || NormalIndex < sizeof...(TAttributes));
		static_assert(TexCoordsIndex == -1 || TexCoordsIndex < sizeof...(TAttributes));

		public:

			using IsVertex = bool;
			
			static constexpr uint32_t positionIndex = PositionIndex;
			static constexpr uint32_t normalIndex = NormalIndex;
			static constexpr uint32_t texCoordsIndex = TexCoordsIndex;

			template<uint32_t Index> using AttributeType = std::tuple_element<Index, std::tuple<TAttributes...>>::type;

			static constexpr uint32_t getAttributeCount();
			static constexpr spl::GlslType getAttributeType(uint32_t i);
			static constexpr uint32_t getAttributeOffsetInStructure(uint32_t i);

			constexpr const void* getAttribute(uint32_t i) const;
			constexpr void* getAttribute(uint32_t i);

			constexpr void setPosition(float x, float y, float z, float w);
			constexpr void setNormal(float x, float y, float z, float w);
			constexpr void setTexCoords(float x, float y, float z, float w);

			constexpr ~VertexBase() = default;

		protected:

			constexpr VertexBase() = default;

			constexpr VertexBase(const VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>& vertex) = default;
			constexpr VertexBase(VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>&& vertex) = default;

			constexpr VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>& operator=(const VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>& vertex) = default;
			constexpr VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>& operator=(VertexBase<PositionIndex, NormalIndex, TexCoordsIndex, TAttributes...>&& vertex) = default;


			template<uint32_t Index> constexpr void _setVector(float x, float y, float z, float w);
	};

	#pragma pack(push, 1)
	struct DefaultVertex : public VertexBase<0, 1, 2, scp::f32vec3, scp::f32vec3, scp::f32vec2>
	{
		constexpr DefaultVertex() = default;
		constexpr DefaultVertex(const scp::f32vec3& pos, const scp::f32vec3& n, const scp::f32vec2& tex);
		constexpr DefaultVertex(const DefaultVertex& vertex) = default;
		constexpr DefaultVertex(DefaultVertex&& vertex) = default;

		constexpr DefaultVertex& operator=(const DefaultVertex& vertex) = default;
		constexpr DefaultVertex& operator=(DefaultVertex&& vertex) = default;

		constexpr ~DefaultVertex() = default;

		scp::f32vec3 position;
		scp::f32vec3 normal;
		scp::f32vec2 texCoords;
	};
	#pragma pack(pop)
}
