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

	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut) : LightBase()
	{
		setPosition(x, y, z);
		setDirection(xDir, yDir, zDir);
		setAngularRadii(angularRadiusIn, angularRadiusOut);
	}

	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity)
	{
		setPosition(x, y, z);
		setDirection(xDir, yDir, zDir);
		setAngularRadii(angularRadiusIn, angularRadiusOut);
	}

	constexpr void LightSpot::setAngularRadii(float in, float out)
	{
		assert(in >= 0.f && in <= std::numbers::pi);
		assert(out >= 0.f && out <= std::numbers::pi);
		assert(in <= out);

		_radii.x = in;
		_radii.y = out;
	}

	constexpr LightType LightSpot::getType() const
	{
		return LightType::Spot;
	}

	constexpr const scp::f32vec2& LightSpot::getAngularRadii() const
	{
		return _radii;
	}

	inline void LightSpot::_getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const
	{
		scp::f32vec3 lightDir(0.f, 0.f, -1.f);
		applyRotationTo(lightDir);

		params[0] = view * scp::f32vec4(getPosition(), 1.f);
		params[1] = view * scp::f32vec4{ -lightDir, 0.f };

		params[0].w = std::cos(_radii.x);
		params[1].w = std::cos(_radii.y);
	}
}
