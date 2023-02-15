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
		_material(&Material::defaultMaterial)
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

	const ShaderSet* Drawable::getShaderSet() const
	{
		return _shaderSet;
	}

	const Material* Drawable::getMaterial() const
	{
		return _material;
	}
}
