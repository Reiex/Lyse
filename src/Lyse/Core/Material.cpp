///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

namespace lys
{
	const Material Material::defaultMaterial;

	Material::Material(float r, float g, float b, float a, float ambiant, float metallic, float roughness) :
		_color(1.f, 1.f, 1.f, 1.f),
		_props(0.04f, 0.f, 0.3f),
		_colorTexture(nullptr),
		_propsTexture(nullptr)
	{
		setColor(r, g, b, a);
		setProperties(ambiant, metallic, roughness);
	}

	void Material::setColor(float r, float g, float b, float a)
	{
		assert(r >= 0.f && r <= 1.f);
		assert(g >= 0.f && g <= 1.f);
		assert(b >= 0.f && b <= 1.f);
		assert(a >= 0.f && a <= 1.f);

		_color.x = r;
		_color.y = g;
		_color.z = b;
		_color.w = a;
	}

	void Material::setProperties(float ambiant, float metallic, float roughness)
	{
		assert(ambiant >= 0.f && ambiant <= 1.f);
		assert(metallic >= 0.f && metallic <= 1.f);
		assert(roughness >= 0.f && roughness <= 1.f);

		_props.x = ambiant;
		_props.y = metallic;
		_props.z = roughness;
	}

	void Material::setColorTexture(const spl::Texture2D* texture)
	{
		_colorTexture = texture;
	}

	void Material::setPropertiesTexture(const spl::Texture2D* texture)
	{
		_propsTexture = texture;
	}

	const scp::f32vec4& Material::getColor() const
	{
		return _color;
	}

	const scp::f32vec3& Material::getProperties() const
	{
		return _props;
	}

	const spl::Texture2D* Material::getColorTexture() const
	{
		return _colorTexture;
	}

	const spl::Texture2D* Material::getPropertiesTexture() const
	{
		return _propsTexture;
	}
}
