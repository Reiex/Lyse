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

#if defined(_WIN32)
	#ifdef LYSE_EXPORTS
		#define LYS_API __declspec(dllexport)
	#else
		#define LYS_API __declspec(dllimport)
	#endif
#elif defined(__linux__)
	#define LYS_API
#else
	#error "Unrecognized platform"
#endif

namespace lys
{
	template<typename T> concept CVertexAttribute = spl::CGlslScalarType<T> || spl::CGlslVecType<T>;
	template<typename T> concept CBufferStorageSpecifier = std::same_as<T, spl::BufferUsage> || std::same_as<T, spl::BufferStorageFlags::Flags>;


	class Transformable;
	
	enum class CameraType;
	class CameraBase;
	class CameraPerspective;
	class CameraOrthographic;

	enum class LightType;
	class LightBase;
	class LightPoint;
	class LightSun;
	class LightSpot;


	enum class ShaderType;
	namespace shaderInterface
	{
		struct SubInterfaceDrawable;
		struct SubInterfaceGBufferResult;
		struct SubInterfaceShadowResult;
		struct SubInterfaceTransparencyResult;
		struct SubInterfaceSsaoResult;
		struct InterfaceGBuffer;
		struct InterfaceShadow;
		struct InterfaceTransparency;
	}
	class ShaderSet;

	class Material;

	enum class DrawableType;
	class Drawable;

	class DrawableGroup;


	struct VertexSpecialIndices
	{
		const uint32_t position;
		const uint32_t normal;
		const uint32_t tangent;
		const uint32_t texCoords;
	};
	template<VertexSpecialIndices SpecialIndices, CVertexAttribute... TAttributes> class VertexBase;
	template<typename T> concept CVertex = requires { typename T::IsVertex; };
	struct VertexDefaultMesh;


	enum class MeshFormat;
	template<CVertex TVertex> class Mesh;


	struct SceneParameters;
	class Scene;
}
