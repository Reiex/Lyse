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
	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angleIn, float angleOut) : LightBase(),
		_camera(1.f, angleOut, 0.01f, 100.f),
		_angleIn(angleIn)
	{
		_camera.setPosition(x, y, z);
		_camera.setDirection(xDir, yDir, zDir);
	}

	constexpr LightSpot::LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angleIn, float angleOut, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_camera(1.f, angleOut, 0.01f, 100.f),
		_angleIn(angleIn)
	{
		_camera.setPosition(x, y, z);
		_camera.setDirection(xDir, yDir, zDir);
	}

	constexpr void LightSpot::setPosition(const scp::f32vec3& position)
	{
		_camera.setPosition(position);
	}

	constexpr void LightSpot::setPosition(float x, float y, float z)
	{
		_camera.setPosition(x, y, z);
	}

	constexpr void LightSpot::move(const scp::f32vec3& offset)
	{
		_camera.move(offset);
	}

	constexpr void LightSpot::move(float dx, float dy, float dz)
	{
		_camera.move(dx, dy, dz);
	}

	constexpr const void LightSpot::setDirection(const scp::f32vec3& direction)
	{
		_camera.setDirection(direction);
	}

	constexpr const void LightSpot::setDirection(float x, float y, float z)
	{
		_camera.setDirection(x, y, z);
	}

	constexpr void LightSpot::setAngularRadii(float in, float out)
	{
		_angleIn = in;
		_camera.setFieldOfView(out);
	}

	constexpr LightType LightSpot::getType() const
	{
		return LightType::Spot;
	}

	constexpr const scp::f32vec3& LightSpot::getPosition() const
	{
		return _camera.getPosition();
	}

	constexpr const scp::f32vec3& LightSpot::getDirection() const
	{
		return _camera.getFrontVector();
	}

	constexpr float LightSpot::getAngleIn() const
	{
		return _angleIn;
	}

	constexpr float LightSpot::getAngleOut() const
	{
		return _camera.getFieldOfView();
	}

	inline void LightSpot::_getUboParams(const CameraBase* camera, scp::f32vec4* params) const
	{
		params[0] = camera->getViewMatrix() * scp::f32vec4(_camera.getPosition(), 1.f);
		params[1] = camera->getViewMatrix() * scp::f32vec4(-_camera.getFrontVector(), 0.f);
		
		params[0].w = std::cos(_angleIn);
		params[1].w = std::cos(_camera.getFieldOfView());
	}

	constexpr void LightSpot::_getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const
	{
		shadowCameras.push_back(&_camera);
	}
}
