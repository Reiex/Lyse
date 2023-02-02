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


	class Transformable;
	
	class CameraBase;
	class CameraPerspective;


	class SceneDrawable;
	class Drawable;

	template<CVertexAttribute TAttribute, CVertexAttribute... TNextAttributes> class Vertex;
	template<typename T> concept CVertex = requires { typename T::IsVertex; };
	using DefaultVertex = Vertex<scp::f32vec3, scp::f32vec3, scp::f32vec2>;
	template<CVertex TVertex> class Mesh;

	class Scene;
}
