///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

namespace lys
{
	Drawable::Drawable() : Transformable(),
		_shaderSet(nullptr),
		_material(&Material::defaultMaterial),
		_geometryCullingMode(spl::FaceCullingMode::CounterClockWise),
		_geometryCullingOrientation(spl::FaceOrientation::Back),
		_shadowCullingMode(spl::FaceCullingMode::CounterClockWise),
		_shadowCullingOrientation(spl::FaceOrientation::Front),
		_shadowBias(0.f)
	{
	}

	void Drawable::setShaderSet(const ShaderSet* shaderSet)
	{
		_shaderSet = shaderSet;
	}

	void Drawable::setMaterial(const Material* material)
	{
		assert(material != nullptr);
		_material = material;
	}

	void Drawable::setGeometryFaceCullingMode(spl::FaceCullingMode mode)
	{
		_geometryCullingMode = mode;
	}

	void Drawable::setGeometryFaceCullingOrientation(spl::FaceOrientation orientation)
	{
		_geometryCullingOrientation = orientation;
	}

	void Drawable::setShadowFaceCullingMode(spl::FaceCullingMode mode)
	{
		_shadowCullingMode = mode;
	}

	void Drawable::setShadowFaceCullingOrientation(spl::FaceOrientation orientation)
	{
		_shadowCullingOrientation = orientation;
	}

	void Drawable::setShadowBias(float bias)
	{
		_shadowBias = bias;
	}

	const ShaderSet* Drawable::getShaderSet() const
	{
		return _shaderSet;
	}

	const Material* Drawable::getMaterial() const
	{
		return _material;
	}

	spl::FaceCullingMode Drawable::getGeometryFaceCullingMode() const
	{
		return _geometryCullingMode;
	}

	spl::FaceOrientation Drawable::getGeometryFaceCullingOrientation() const
	{
		return _geometryCullingOrientation;
	}

	spl::FaceCullingMode Drawable::getShadowFaceCullingMode() const
	{
		return _shadowCullingMode;
	}

	spl::FaceOrientation Drawable::getShadowFaceCullingOrientation() const
	{
		return _shadowCullingOrientation;
	}

	float Drawable::getShadowBias() const
	{
		return _shadowBias;
	}
}
