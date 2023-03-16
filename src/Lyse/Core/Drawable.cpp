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
		_geometryCulling(spl::FaceCulling::FrontClockWise),
		_shadowCulling(spl::FaceCulling::BackClockWise),
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

	void Drawable::setGeometryFaceCulling(spl::FaceCulling culling)
	{
		_geometryCulling = culling;
	}

	void Drawable::setShadowFaceCulling(spl::FaceCulling culling)
	{
		_shadowCulling = culling;
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

	spl::FaceCulling Drawable::getGeometryFaceCulling() const
	{
		return _geometryCulling;
	}

	spl::FaceCulling Drawable::getShadowFaceCulling() const
	{
		return _shadowCulling;
	}

	float Drawable::getShadowBias() const
	{
		return _shadowBias;
	}
}
