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
	constexpr LightPoint::LightPoint(float x, float y, float z) : LightBase(),
		_position(x, y, z)
	{
	}

	constexpr LightPoint::LightPoint(float x, float y, float z, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_position(x, y, z)
	{
	}

	constexpr void LightPoint::setPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	constexpr LightType LightPoint::getType() const
	{
		return LightType::Point;
	}

	constexpr const scp::f32vec3& LightPoint::getPosition() const
	{
		return _position;
	}

	constexpr void LightPoint::_getParams(scp::f32vec4* params) const
	{
		params[0].x = _position.x;
		params[0].y = _position.y;
		params[0].z = _position.z;
	}
}
