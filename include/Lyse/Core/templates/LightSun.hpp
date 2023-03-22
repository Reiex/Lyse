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
		_cameras({ CameraOrthographic(1.f, 1.f, 1.f) })
	{
		_cameras.front().setDirection(xDir, yDir, zDir);
	}

	inline LightSun::LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity) : LightBase(r, g, b, intensity),
		_cameras({ CameraOrthographic(1.f, 1.f, 1.f) })
	{
		_cameras.front().setDirection(xDir, yDir, zDir);
	}

	constexpr const void LightSun::setDirection(const scp::f32vec3& direction)
	{
		_cameras.front().setDirection(direction);
		_cameras.resize(1);
	}

	constexpr const void LightSun::setDirection(float x, float y, float z)
	{
		_cameras.front().setDirection(x, y, z);
		_cameras.resize(1);
	}

	constexpr LightType LightSun::getType() const
	{
		return LightType::Sun;
	}

	constexpr const scp::f32vec3& LightSun::getDirection() const
	{
		return _cameras.front().getFrontVector();
	}

	constexpr void LightSun::_getUboParams(const CameraBase* camera, scp::f32vec4* params) const
	{
		params[0] = camera->getViewMatrix() * scp::f32vec4(-_cameras.front().getFrontVector(), 0.f);
	}

	inline void LightSun::_getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const
	{
		// Compute camera frustum corner positions in light view-space

		scp::f32vec4 frustumCorners[8] = {
			{-1.f, -1.f, -1.f,  1.f},
			{ 1.f, -1.f, -1.f,  1.f},
			{-1.f,  1.f, -1.f,  1.f},
			{ 1.f,  1.f, -1.f,  1.f},
			{-1.f, -1.f,  1.f,  1.f},
			{ 1.f, -1.f,  1.f,  1.f},
			{-1.f,  1.f,  1.f,  1.f},
			{ 1.f,  1.f,  1.f,  1.f},
		};

		for (uint8_t i = 0; i < 8; ++i)
		{
			frustumCorners[i] = camera->getInverseProjectionViewMatrix() * frustumCorners[i];
			frustumCorners[i].xyz() /= frustumCorners[i].w;
			_cameras.front().applyInverseRotationTo(frustumCorners[i].xyz());
		}

		// Compute a camera for each element of the shadow cascade

		if (_cameras.size() != _shadowCascadeSize)
		{
			_cameras.resize(_shadowCascadeSize, _cameras.front());
		}

		const float near = camera->getNearDistance();
		const float far = camera->getFarDistance();
		const float invShadowCascadeSize = 1.f / _shadowCascadeSize;
		float s = 0.f;
		for (uint32_t i = 0; i < _shadowCascadeSize; ++i)
		{
			const float t = (1.f - invShadowCascadeSize) * (near * std::pow(far / near, (i + 1) * invShadowCascadeSize) - near) / (far - near) + invShadowCascadeSize * (i + 1) * invShadowCascadeSize;

			scp::f32vec4 p[8] = {
				frustumCorners[0] * (1.f - s) + frustumCorners[4] * s,
				frustumCorners[1] * (1.f - s) + frustumCorners[5] * s,
				frustumCorners[2] * (1.f - s) + frustumCorners[6] * s,
				frustumCorners[3] * (1.f - s) + frustumCorners[7] * s,
				frustumCorners[0] * (1.f - t) + frustumCorners[4] * t,
				frustumCorners[1] * (1.f - t) + frustumCorners[5] * t,
				frustumCorners[2] * (1.f - t) + frustumCorners[6] * t,
				frustumCorners[3] * (1.f - t) + frustumCorners[7] * t
			};

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
				bbox[5] + 100.f	// TODO : Better than that magic number
			};

			scp::f32vec3 size = {
				bbox[1] - bbox[0],
				bbox[3] - bbox[2],
				position.z - bbox[4]
			};

			_cameras.front().applyRotationTo(position);

			_cameras[i].setSize(size.x, size.y, size.z);
			_cameras[i].setPosition(position);

			shadowCameras.push_back(&_cameras[i]);

			s = t;
		}
	}
}
