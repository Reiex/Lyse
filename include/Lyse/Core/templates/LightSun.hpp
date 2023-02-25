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
	inline LightSun::LightSun(float xDir, float yDir, float zDir) : LightBase(),
		_camera(1.f, 1.f, 1.f)
	{
		_camera.setDirection(xDir, yDir, zDir);
	}

	inline LightSun::LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_camera(1.f, 1.f, 1.f)
	{
		_camera.setDirection(xDir, yDir, zDir);
	}

	constexpr const void LightSun::setDirection(const scp::f32vec3& direction)
	{
		_camera.setDirection(direction);
	}

	constexpr const void LightSun::setDirection(float x, float y, float z)
	{
		_camera.setDirection(x, y, z);
	}

	constexpr LightType LightSun::getType() const
	{
		return LightType::Sun;
	}

	constexpr const scp::f32vec3& LightSun::getDirection() const
	{
		return _camera.getFrontVector();
	}

	constexpr void LightSun::_getUboParams(const CameraBase* camera, scp::f32vec4* params) const
	{
		params[0] = camera->getViewMatrix() * scp::f32vec4(-_camera.getFrontVector(), 0.f);
	}

	inline void LightSun::_getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const
	{
		scp::f32vec4 p[8] = {
			{-1.f, -1.f, -1.f,  1.f},
			{ 1.f, -1.f, -1.f,  1.f},
			{-1.f,  1.f, -1.f,  1.f},
			{ 1.f,  1.f, -1.f,  1.f},
			{-1.f, -1.f,  1.f,  1.f},
			{ 1.f, -1.f,  1.f,  1.f},
			{-1.f,  1.f,  1.f,  1.f},
			{ 1.f,  1.f,  1.f,  1.f}
		};

		for (uint8_t i = 0; i < 8; ++i)
		{
			p[i] = camera->getInverseProjectionViewMatrix() * p[i];
			p[i].xyz() /= p[i].w;
			_camera.applyInverseRotationTo(p[i].xyz());
		}

		std::array<float, 6> bbox = { p[0].x, p[0].x, p[0].y, p[0].y, p[0].z, p[0].z };
		for (uint8_t i = 1; i < 8; ++i)
		{
			bbox[0] = std::min(bbox[0], p[i].x);
			bbox[1] = std::max(bbox[1], p[i].x);
			bbox[2] = std::min(bbox[2], p[i].y);
			bbox[3] = std::max(bbox[3], p[i].y);
			bbox[4] = std::min(bbox[4], p[i].z);
			bbox[5] = std::max(bbox[5], p[i].z);
		}

		scp::f32vec3 position = {
			(bbox[0] + bbox[1]) / 2.f,
			(bbox[2] + bbox[3]) / 2.f,
			bbox[5] + 10.f	// TODO : Better than that magic number
		};

		scp::f32vec3 size = {
			bbox[1] - bbox[0],
			bbox[3] - bbox[2],
			position.z - bbox[4]
		};

		_camera.applyRotationTo(position);

		_camera.setSize(size.x, size.y, size.z);
		_camera.setPosition(position);

		shadowCameras.push_back(&_camera);
	}
}
