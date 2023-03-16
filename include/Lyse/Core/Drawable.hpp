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
			void setGeometryFaceCulling(spl::FaceCulling culling);
			void setShadowFaceCulling(spl::FaceCulling culling);
			void setShadowBias(float bias);

			virtual DrawableType getType() const = 0;
			const ShaderSet* getShaderSet() const;
			const Material* getMaterial() const;
			spl::FaceCulling getGeometryFaceCulling() const;
			spl::FaceCulling getShadowFaceCulling() const;
			float getShadowBias() const;

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
			spl::FaceCulling _geometryCulling;
			spl::FaceCulling _shadowCulling;
			float _shadowBias;

		friend class Scene;
	};
}
