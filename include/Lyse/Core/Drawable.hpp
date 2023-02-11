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
	enum class DrawableType : uint8_t
	{
		Particles,
		Mesh,
		MeshInstanced,
		Group
	};

	namespace DrawableFlags
	{
		enum Flags : uint64_t
		{
			None
			/*

			HasNormalMap,
			HasDisplacementMap,

			Shadows ?
			MaterialTypes ?
			etc...

			*/
		};
	}

	struct DrawableShaderSet
	{
		const spl::ShaderProgram* gBufferShader;
		const spl::ShaderProgram* mergeShader;
	};

	struct DrawableInfo
	{
		DrawableType type;
		DrawableFlags::Flags flags;

		DrawableShaderSet shaderSet;
	};

	struct DrawContext
	{
		const spl::ShaderProgram* program;
		scp::f32mat4x4 transform;
	};

	class Drawable : public Transformable
	{
		public:

			Drawable(const Drawable& drawable) = delete;
			Drawable(Drawable&& drawable) = delete;

			Drawable& operator=(const Drawable& drawable) = delete;
			Drawable& operator=(Drawable&& drawable) = delete;

			virtual ~Drawable() override = default;

		protected:

			Drawable() = default;

		private:

			virtual const DrawableInfo& _getInfo() const = 0;

			virtual void _draw(const DrawContext& context) const = 0;

		friend class Scene;
	};
}
