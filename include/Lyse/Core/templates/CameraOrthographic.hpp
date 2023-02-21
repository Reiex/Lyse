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
	constexpr CameraOrthographic::CameraOrthographic(float width, float height, float depth, float fov) :
		_aspect(width / height),
		_fov(fov),
		_depth(depth),
		_projection()
	{
		assert(width > 0.f);
		assert(height > 0.f);
		assert(depth > 0.f);

		if (fov == -1.f)
		{
			_fov = 2.f * std::atan(0.5f * height / _depth);
		}
		else
		{
			assert(fov > 0.f && fov < std::numbers::pi);
		}
	}

	constexpr void CameraOrthographic::setAspect(float aspect)
	{
		assert(aspect > 0.f);

		_aspect = aspect;
		_projection.reset();
	}

	constexpr void CameraOrthographic::setAspect(uint32_t width, uint32_t height)
	{
		assert(width != 0 && height != 0);

		_aspect = static_cast<float>(width) / static_cast<float>(height);
		_projection.reset();
	}

	constexpr void CameraOrthographic::setFieldOfView(float fov)
	{
		assert(fov > 0.f && fov < std::numbers::pi);

		_fov = fov;
		_projection.reset();
	}

	constexpr void CameraOrthographic::setDepth(float depth)
	{
		assert(depth > 0.f);

		_depth = depth;
		_projection.reset();
	}

	constexpr CameraType CameraOrthographic::getType() const
	{
		return CameraType::Orthographic;
	}

	constexpr float CameraOrthographic::getAspect() const
	{
		return _aspect;
	}

	constexpr float CameraOrthographic::getFieldOfView() const
	{
		return _fov;
	}

	constexpr float CameraOrthographic::getNearDistance() const
	{
		return 0.f;
	}

	constexpr float CameraOrthographic::getFarDistance() const
	{
		return _depth;
	}

	constexpr const scp::f32mat4x4& CameraOrthographic::getProjectionMatrix() const
	{
		if (!_projection.has_value())
		{
			const float height = 2.f * _depth * std::tan(_fov * 0.5f);
			const float width = _aspect * height;
			const float a = 2.f / width;
			const float b = 2.f / height;
			const float c = -2.f / _depth;

			_projection.emplace({
				a  , 0.f, 0.f, 0.f,
				0.f, b  , 0.f, 0.f,
				0.f, 0.f, c  , -1.f,
				0.f, 0.f, 0.f, 1.f
				});
		}

		return *_projection;
	}
}
