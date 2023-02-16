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

	Material::Material() :
		_color(1.f, 1.f, 1.f, 1.f),
		_props(0.04f, 0.f, 0.3f),
		_colorTexture(nullptr),
		_propsTexture(nullptr)
	{
	}

	Material::Material(float red, float green, float blue, float alpha, float ambiant, float metallic, float roughness) : Material()
	{
		setColor(red, green, blue, alpha);
		setProperties(ambiant, metallic, roughness);
	}

	void Material::setColor(float red, float green, float blue, float alpha)
	{
		assert(red >= 0.f && red <= 1.f);
		assert(green >= 0.f && green <= 1.f);
		assert(blue >= 0.f && blue <= 1.f);
		assert(alpha >= 0.f && alpha <= 1.f);

		_color.x = red;
		_color.y = green;
		_color.z = blue;
		_color.w = alpha;
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
