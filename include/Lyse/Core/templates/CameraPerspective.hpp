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
	constexpr CameraPerspective::CameraPerspective(float aspect, float near, float far, float fov) :
		_aspect(aspect),
		_near(near),
		_far(far),
		_fov(fov),
		_projection()
	{
		assert(aspect > 0.f);
		assert(near > 0.f);
		assert(far > near);
		assert(fov > 0.f && fov < std::numbers::pi / 2);
	}

	constexpr CameraPerspective::CameraPerspective(uint32_t width, uint32_t height, float near, float far, float fov) : CameraPerspective(static_cast<float>(width) / static_cast<float>(height), near, far, fov)
	{
	}

	constexpr void CameraPerspective::setAspect(float aspect)
	{
		assert(aspect > 0.f);

		_aspect = aspect;
		_projection.reset();
	}

	constexpr void CameraPerspective::setAspect(uint32_t width, uint32_t height)
	{
		assert(width != 0 && height != 0);

		_aspect = static_cast<float>(width) / static_cast<float>(height);
		_projection.reset();
	}

	constexpr void CameraPerspective::setNearDistance(float near)
	{
		assert(near > 0.f);

		_near = near;
		_projection.reset();
	}

	constexpr void CameraPerspective::setFarDistance(float far)
	{
		assert(far > 0.f);

		_far = far;
		_projection.reset();
	}

	constexpr void CameraPerspective::setFieldOfView(float fov)
	{
		assert(fov > 0.f && fov < std::numbers::pi / 2);

		_fov = fov;
		_projection.reset();
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
}
