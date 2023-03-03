///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreDecl.hpp>

namespace lys
{
	constexpr LightBase::LightBase(float r, float g, float b, float intensity) :
		_castShadows(false),
		_color(1.f, 1.f, 1.f),
		_intensity(1.f)
	{
		setColor(r, g, b);
		setIntensity(intensity);
	}

	constexpr void LightBase::setCastShadows(bool castShadows)
	{
		_castShadows = castShadows;
	}

	constexpr void LightBase::setColor(float r, float g, float b)
	{
		assert(r >= 0.f && r <= 1.f);
		assert(g >= 0.f && g <= 1.f);
		assert(b >= 0.f && b <= 1.f);

		_color.x = r;
		_color.y = g;
		_color.z = b;
	}

	constexpr void LightBase::setIntensity(float intensity)
	{
		assert(intensity >= 0.f);

		_intensity = intensity;
	}

	constexpr bool LightBase::getCastShadows() const
	{
		return _castShadows;
	}

	constexpr const scp::f32vec3& LightBase::getColor() const
	{
		return _color;
	}

	constexpr float LightBase::getIntensity() const
	{
		return _intensity;
	}
}
