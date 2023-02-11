///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SciPP/SciPPTypes.hpp>
#include <Diskon/DiskonTypes.hpp>
#include <DejaVu/DejaVuTypes.hpp>
#include <SplayLibrary/SplayLibraryTypes.hpp>

namespace lys
{
	template<typename T> concept CVertexAttribute = spl::CGlslScalarType<T> || spl::CGlslVecType<T>;
	template<typename T> concept CBufferStorageSpecifier = std::same_as<T, spl::BufferUsage> || std::same_as<T, spl::BufferStorageFlags::Flags>;


	class Transformable;
	
	class CameraBase;
	class CameraPerspective;


	enum class DrawableType : uint8_t;
	namespace DrawableFlags { enum Flags : uint64_t; }
	struct DrawableInfo;
	struct DrawContext;
	class Drawable;

	class DrawableGroup;

	template<uint32_t PositionIndex, uint32_t NormalIndex, uint32_t TexCoordsIndex, CVertexAttribute... TAttributes> class VertexBase;
	template<typename T> concept CVertex = requires { typename T::IsVertex; };
	struct DefaultVertex;

	enum class MeshFormat;
	template<CVertex TVertex> class Mesh;

	class Scene;
}
