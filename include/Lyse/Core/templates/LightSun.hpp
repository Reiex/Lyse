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
	constexpr LightSun::LightSun(float xDir, float yDir, float zDir) : LightBase()
	{
		setDirection(xDir, yDir, zDir);
	}

	constexpr LightSun::LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity)
	{
		setDirection(xDir, yDir, zDir);
	}

	constexpr LightType LightSun::getType() const
	{
		return LightType::Sun;
	}

	constexpr void LightSun::_getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const
	{
		scp::f32vec3 lightDir(0.f, 0.f, -1.f);
		applyRotationTo(lightDir);

		params[0] = view * scp::f32vec4{ -lightDir, 0.f };
	}
}
