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
	enum class DrawableType
	{
		Particles,
		Mesh,
		MeshInstanced,
		Group
	};

	struct DrawableShaderSet
	{
		const spl::ShaderProgram* gBufferShader;
	};

	struct DrawContext
	{
		const spl::ShaderProgram* shader;
		scp::f32mat4x4 transform;
	};

	class Drawable : public Transformable
	{
		protected:

			Drawable() = default;

			Drawable(const Drawable& drawable) = default;
			Drawable(Drawable&& drawable) = default;

			Drawable& operator=(const Drawable& drawable) = default;
			Drawable& operator=(Drawable&& drawable) = default;

			virtual ~Drawable() override = default;

		private:

			virtual DrawableType _getType() const = 0;
			virtual const DrawableShaderSet& _getShaderSet() const = 0;
			virtual const Material& _getMaterial() const = 0;

			virtual void _draw(const DrawContext& context) const = 0;

		friend class Scene;
	};
}
