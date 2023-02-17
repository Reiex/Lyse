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

	class Drawable : public Transformable
	{
		public:

			void setShaderSet(const ShaderSet* shaderSet);
			void setMaterial(const Material* material);

			const ShaderSet* getShaderSet() const;
			const Material* getMaterial() const;
			virtual DrawableType getType() const = 0;

		protected:


			Drawable();
			Drawable(const Drawable& drawable) = default;
			Drawable(Drawable&& drawable) = default;

			Drawable& operator=(const Drawable& drawable) = default;
			Drawable& operator=(Drawable&& drawable) = default;

			virtual void _draw() const = 0;

			virtual ~Drawable() override = default;

			const ShaderSet* _shaderSet;
			const Material* _material;

		friend class Scene;
	};
}
