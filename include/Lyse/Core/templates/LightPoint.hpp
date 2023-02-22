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
	constexpr LightPoint::LightPoint(float x, float y, float z) : LightBase()
	{
		setPosition(x, y, z);
	}

	constexpr LightPoint::LightPoint(float x, float y, float z, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity)
	{
		setPosition(x, y, z);
	}

	constexpr LightType LightPoint::getType() const
	{
		return LightType::Point;
	}

	constexpr void LightPoint::_getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const
	{
		params[0] = view * scp::f32vec4(getPosition(), 1.f);
	}
}
