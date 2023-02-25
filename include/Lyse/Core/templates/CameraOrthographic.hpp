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
	inline CameraOrthographic::CameraOrthographic(float width, float height, float depth) : CameraBase()
	{
		setSize(width, height, depth);
	}

	inline void CameraOrthographic::setSize(float width, float height, float depth)
	{
		assert(width > 0.f);
		assert(height > 0.f);
		assert(depth > 0.f);

		_uboCameraData.aspect = width / height;
		_uboCameraData.near = 0.f;
		_uboCameraData.far = depth;
		_uboCameraData.fov = 2.f * std::atan(0.5f * height / depth);

		_projectionDependentNeedsUpdate();
	}

	constexpr CameraType CameraOrthographic::getType() const
	{
		return CameraType::Orthographic;
	}

	inline void CameraOrthographic::_updateProjectionMatrix() const
	{
		const float height = _uboCameraData.far * std::tan(_uboCameraData.fov * 0.5f);
		const float width = _uboCameraData.aspect * height;
		const float a = 1.f / width;
		const float b = 1.f / height;
		const float c = -2.f / _uboCameraData.far;

		_uboCameraData.projection = {
			a  , 0.f, 0.f,  0.f,
			0.f, b  , 0.f,  0.f,
			0.f, 0.f, c  , -1.f,
			0.f, 0.f, 0.f,  1.f
			};
	}

	inline void CameraOrthographic::_updateInverseProjectionMatrix() const
	{
		const float b = _uboCameraData.far * std::tan(_uboCameraData.fov * 0.5f);
		const float a = _uboCameraData.aspect * b;
		const float c = -_uboCameraData.far * 0.5f;

		_uboCameraData.invProjection = {
			a  , 0.f, 0.f, 0.f,
			0.f, b  , 0.f, 0.f,
			0.f, 0.f, c  , c  ,
			0.f, 0.f, 0.f, 1.f
			};
	}
}
