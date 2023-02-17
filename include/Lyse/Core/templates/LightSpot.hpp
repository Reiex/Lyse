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

	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut) : LightBase(),
		_position(x, y, z),
		_dir(xDir, yDir, zDir)
	{
		setAngularRadii(angularRadiusIn, angularRadiusOut);
	}

	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_position(x, y, z),
		_dir(xDir, yDir, zDir)
	{
		setAngularRadii(angularRadiusIn, angularRadiusOut);
	}

	constexpr void LightSpot::setPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	constexpr void LightSpot::setLightDir(float x, float y, float z)
	{
		_dir.x = x;
		_dir.y = y;
		_dir.z = z;
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

	constexpr const scp::f32vec3& LightSpot::getPosition() const
	{
		return _position;
	}

	constexpr const scp::f32vec3& LightSpot::getLightDir() const
	{
		return _dir;
	}

	constexpr const scp::f32vec2& LightSpot::getAngularRadii() const
	{
		return _radii;
	}

	inline void LightSpot::_getParams(scp::f32vec4* params) const
	{
		scp::f32vec3 tmp = -scp::normalize(_dir);

		params[0].x = _position.x;
		params[0].y = _position.y;
		params[0].z = _position.z;
		params[0].w = std::cos(_radii.x);

		params[1].x = tmp.x;
		params[1].y = tmp.y;
		params[1].z = tmp.z;
		params[1].w = std::cos(_radii.y);
	}
}
