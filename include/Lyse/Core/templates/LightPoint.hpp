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

	constexpr void LightPoint::setPosition(const scp::f32vec3& position)
	{
		_position = position;
	}

	constexpr void LightPoint::setPosition(float x, float y, float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	constexpr void LightPoint::move(const scp::f32vec3& offset)
	{
		_position += offset;
	}

	constexpr void LightPoint::move(float dx, float dy, float dz)
	{
		_position.x += dx;
		_position.y += dy;
		_position.z += dz;
	}

	constexpr LightType LightPoint::getType() const
	{
		return LightType::Point;
	}

	constexpr const scp::f32vec3& LightPoint::getPosition() const
	{
		return _position;
	}

	constexpr void LightPoint::_getUboParams(const CameraBase* camera, scp::f32vec4* params) const
	{
		params[0] = camera->getViewMatrix() * scp::f32vec4(getPosition(), 1.f);
	}

	constexpr void LightPoint::_getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const
	{
		// TODO
	}
}
