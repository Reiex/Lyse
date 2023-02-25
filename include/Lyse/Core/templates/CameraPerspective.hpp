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
	constexpr CameraPerspective::CameraPerspective(float aspect, float fov, float near, float far) : CameraBase()
	{
		assert(aspect > 0.f);
		assert(near > 0.f);
		assert(far > near);
		assert(fov > 0.f && fov < std::numbers::pi);

		_uboCameraData.aspect = aspect;
		_uboCameraData.fov = fov;
		_uboCameraData.near = near;
		_uboCameraData.far = far;
	}

	constexpr CameraPerspective::CameraPerspective(uint32_t width, uint32_t height, float fov, float near, float far) : CameraPerspective(static_cast<float>(width) / static_cast<float>(height), fov, near, far)
	{
	}

	constexpr void CameraPerspective::setAspect(float aspect)
	{
		assert(aspect > 0.f);

		_uboCameraData.aspect = aspect;
		_projectionDependentNeedsUpdate();
	}

	constexpr void CameraPerspective::setAspect(uint32_t width, uint32_t height)
	{
		assert(width != 0 && height != 0);

		_uboCameraData.aspect = static_cast<float>(width) / static_cast<float>(height);
		_projectionDependentNeedsUpdate();
	}

	constexpr void CameraPerspective::setNearDistance(float near)
	{
		assert(near > 0.f);

		_uboCameraData.near = near;
		_projectionDependentNeedsUpdate();
	}

	constexpr void CameraPerspective::setFarDistance(float far)
	{
		assert(far > 0.f);

		_uboCameraData.far = far;
		_projectionDependentNeedsUpdate();
	}

	constexpr void CameraPerspective::setFieldOfView(float fov)
	{
		assert(fov > 0.f && fov < std::numbers::pi);

		_uboCameraData.fov = fov;
		_projectionDependentNeedsUpdate();
	}

	constexpr CameraType CameraPerspective::getType() const
	{
		return CameraType::Perspective;
	}

	inline void CameraPerspective::_updateProjectionMatrix() const
	{
		const float invTanFov = 1.f / std::tan(_uboCameraData.fov * 0.5f);
		const float a = invTanFov / _uboCameraData.aspect;
		const float b = invTanFov;
		const float invDepth = 1.f / (_uboCameraData.near - _uboCameraData.far);
		const float c = (_uboCameraData.near + _uboCameraData.far) * invDepth;
		const float d = (2.f * _uboCameraData.near * _uboCameraData.far) * invDepth;

		_uboCameraData.projection = {
			a  , 0.f,  0.f, 0.f,
			0.f, b  ,  0.f, 0.f,
			0.f, 0.f,  c  , d  ,
			0.f, 0.f, -1.f, 0.f
			};
	}

	inline void CameraPerspective::_updateInverseProjectionMatrix() const
	{
		const float b = std::tan(_uboCameraData.fov * 0.5);
		const float a = _uboCameraData.aspect * b;
		const float tmp = 1.f / (2.f * _uboCameraData.near * _uboCameraData.far);
		const float c = (_uboCameraData.near - _uboCameraData.far) * tmp;
		const float d = (_uboCameraData.near + _uboCameraData.far) * tmp;

		_uboCameraData.invProjection = {
			a  , 0.f, 0.f,  0.f,
			0.f, b  , 0.f,  0.f,
			0.f, 0.f, 0.f, -1.f,
			0.f, 0.f, c  ,  d
			};
	}
}
