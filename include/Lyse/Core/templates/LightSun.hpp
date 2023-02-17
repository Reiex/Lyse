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
	constexpr LightSun::LightSun(float xDir, float yDir, float zDir) : LightBase(),
		_dir(xDir, yDir, zDir)
	{
	}

	constexpr LightSun::LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_dir(xDir, yDir, zDir)
	{
	}

	constexpr void LightSun::setLightDir(float x, float y, float z)
	{
		_dir.x = x;
		_dir.y = y;
		_dir.z = z;
	}

	constexpr LightType LightSun::getType() const
	{
		return LightType::Sun;
	}

	constexpr const scp::f32vec3& LightSun::getLightDir() const
	{
		return _dir;
	}

	inline void LightSun::_getParams(scp::f32vec4* params) const
	{
		scp::f32vec3 tmp = -scp::normalize(_dir);

		params[0].x = tmp.x;
		params[0].y = tmp.y;
		params[0].z = tmp.z;
	}
}
