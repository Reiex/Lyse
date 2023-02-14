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
	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes>
	class VertexBase
	{
		static_assert(SpecialIndices.position == UINT32_MAX || SpecialIndices.position < sizeof...(TAttributes));
		static_assert(SpecialIndices.normal == UINT32_MAX || SpecialIndices.normal < sizeof...(TAttributes));
		static_assert(SpecialIndices.tangent == UINT32_MAX || SpecialIndices.tangent < sizeof...(TAttributes));
		static_assert(SpecialIndices.texCoords == UINT32_MAX || SpecialIndices.texCoords < sizeof...(TAttributes));

		public:

			using IsVertex = bool;
			
			static constexpr VertexSpecialIndices specialIndices = SpecialIndices;

			template<uint32_t Index> using AttributeType = std::tuple_element<Index, std::tuple<TAttributes...>>::type;

			static constexpr uint32_t getAttributeCount();
			static constexpr spl::GlslType getAttributeType(uint32_t i);
			static constexpr uint32_t getAttributeOffsetInStructure(uint32_t i);

			constexpr const void* getAttribute(uint32_t i) const;
			constexpr void* getAttribute(uint32_t i);

			constexpr void setPosition(float x, float y, float z, float w);
			constexpr void setNormal(float x, float y, float z, float w);
			constexpr void setTangent(float x, float y, float z, float w);
			constexpr void setTexCoords(float x, float y, float z, float w);

			constexpr void getPosition(float& x, float& y, float& z, float& w) const;
			constexpr void getNormal(float& x, float& y, float& z, float& w) const;
			constexpr void getTangent(float& x, float& y, float& z, float& w) const;
			constexpr void getTexCoords(float& x, float& y, float& z, float& w) const;

			constexpr ~VertexBase() = default;

		protected:

			constexpr VertexBase() = default;

			constexpr VertexBase(const VertexBase<SpecialIndices, TAttributes...>& vertex) = default;
			constexpr VertexBase(VertexBase<SpecialIndices, TAttributes...>&& vertex) = default;

			constexpr VertexBase<SpecialIndices, TAttributes...>& operator=(const VertexBase<SpecialIndices, TAttributes...>& vertex) = default;
			constexpr VertexBase<SpecialIndices, TAttributes...>& operator=(VertexBase<SpecialIndices, TAttributes...>&& vertex) = default;


			template<uint32_t Index> constexpr void _setVector(float x, float y, float z, float w);
			template<uint32_t Index> constexpr void _getVector(float& x, float& y, float& z, float& w) const;
	};

	namespace _lys
	{
		constexpr VertexSpecialIndices NoIndices = { UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT32_MAX };
		constexpr VertexSpecialIndices VertexDefaultMeshIndices = { 0, 1, 2, 3 };
	}

	#pragma pack(push, 1)

	struct VertexDefaultMesh : public VertexBase<_lys::VertexDefaultMeshIndices, scp::f32vec4, scp::f32vec4, scp::f32vec4, scp::f32vec4>
	{
		constexpr VertexDefaultMesh();
		constexpr VertexDefaultMesh(const scp::f32vec4& p, const scp::f32vec4& n, const scp::f32vec4& t, const scp::f32vec4& tc);
		constexpr VertexDefaultMesh(const VertexDefaultMesh& vertex) = default;
		constexpr VertexDefaultMesh(VertexDefaultMesh&& vertex) = default;

		constexpr VertexDefaultMesh& operator=(const VertexDefaultMesh& vertex) = default;
		constexpr VertexDefaultMesh& operator=(VertexDefaultMesh&& vertex) = default;

		constexpr ~VertexDefaultMesh() = default;

		scp::f32vec4 position;
		scp::f32vec4 normal;
		scp::f32vec4 tangent;
		scp::f32vec4 texCoords;
	};

	#pragma pack(pop)
}
