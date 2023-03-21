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
			void setGeometryFaceCullingMode(spl::FaceCullingMode mode);
			void setGeometryFaceCullingOrientation(spl::FaceOrientation orientation);
			void setShadowFaceCullingMode(spl::FaceCullingMode mode);
			void setShadowFaceCullingOrientation(spl::FaceOrientation orientation);
			void setShadowBias(float bias);

			virtual DrawableType getType() const = 0;
			const ShaderSet* getShaderSet() const;
			const Material* getMaterial() const;
			spl::FaceCullingMode getGeometryFaceCullingMode() const;
			spl::FaceOrientation getGeometryFaceCullingOrientation() const;
			spl::FaceCullingMode getShadowFaceCullingMode() const;
			spl::FaceOrientation getShadowFaceCullingOrientation() const;
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
			spl::FaceCullingMode _geometryCullingMode;
			spl::FaceOrientation _geometryCullingOrientation;
			spl::FaceCullingMode _shadowCullingMode;
			spl::FaceOrientation _shadowCullingOrientation;
			float _shadowBias;

		friend class Scene;
	};
}
