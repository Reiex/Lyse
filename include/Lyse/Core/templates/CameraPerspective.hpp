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
	constexpr CameraPerspective::CameraPerspective(float aspect, float fov, float near, float far) :
		_aspect(aspect),
		_fov(fov),
		_near(near),
		_far(far),
		_projection(),
		_invProjection()
	{
		assert(aspect > 0.f);
		assert(near > 0.f);
		assert(far > near);
		assert(fov > 0.f && fov < std::numbers::pi);
	}

	constexpr CameraPerspective::CameraPerspective(uint32_t width, uint32_t height, float fov, float near, float far) : CameraPerspective(static_cast<float>(width) / static_cast<float>(height), fov, near, far)
	{
	}

	constexpr void CameraPerspective::setAspect(float aspect)
	{
		assert(aspect > 0.f);

		_aspect = aspect;
		_projection.reset();
		_invProjection.reset();
	}

	constexpr void CameraPerspective::setAspect(uint32_t width, uint32_t height)
	{
		assert(width != 0 && height != 0);

		_aspect = static_cast<float>(width) / static_cast<float>(height);
		_projection.reset();
		_invProjection.reset();
	}

	constexpr void CameraPerspective::setNearDistance(float near)
	{
		assert(near > 0.f);

		_near = near;
		_projection.reset();
		_invProjection.reset();
	}

	constexpr void CameraPerspective::setFarDistance(float far)
	{
		assert(far > 0.f);

		_far = far;
		_projection.reset();
		_invProjection.reset();
	}

	constexpr void CameraPerspective::setFieldOfView(float fov)
	{
		assert(fov > 0.f && fov < std::numbers::pi);

		_fov = fov;
		_projection.reset();
		_invProjection.reset();
	}

	constexpr CameraType CameraPerspective::getType() const
	{
		return CameraType::Perspective;
	}

	constexpr float CameraPerspective::getAspect() const
	{
		return _aspect;
	}

	constexpr float CameraPerspective::getNearDistance() const
	{
		return _near;
	}

	constexpr float CameraPerspective::getFarDistance() const
	{
		return _far;
	}

	constexpr float CameraPerspective::getFieldOfView() const
	{
		return _fov;
	}

	constexpr const scp::f32mat4x4& CameraPerspective::getProjectionMatrix() const
	{
		if (!_projection.has_value())
		{
			const float invTanFov = 1.f / std::tan(_fov * 0.5f);
			const float a = invTanFov / _aspect;
			const float b = invTanFov;
			const float invDepth = 1.f / (_near - _far);
			const float c = (_near + _far) * invDepth;
			const float d = (2.f * _near * _far) * invDepth;

			_projection.emplace({
				a  , 0.f,  0.f, 0.f,
				0.f, b  ,  0.f, 0.f,
				0.f, 0.f,  c  , d  ,
				0.f, 0.f, -1.f, 0.f
				});
		}

		return *_projection;
	}

	constexpr const scp::f32mat4x4& CameraPerspective::getInverseProjectionMatrix() const
	{
		if (!_invProjection.has_value())
		{
			const float b = std::tan(_fov * 0.5);
			const float a = _aspect * b;
			const float tmp = 1.f / (2.f * _near * _far);
			const float c = (_near - _far) * tmp;
			const float d = (_near + _far) * tmp;

			_invProjection.emplace({
				a  , 0.f, 0.f,  0.f,
				0.f, b  , 0.f,  0.f,
				0.f, 0.f, 0.f, -1.f,
				0.f, 0.f, c  ,  d
				});
		}

		return *_invProjection;
	}
}
