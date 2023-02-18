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
	inline void CameraBase::lookAt(const scp::f32vec3& position, float dutchAngle)
	{
		if (distance(position, getTranslation()) == 0.f)
		{
			return;
		}

		scp::Quat<float> rot = { 1.f, 0.f, 0.f, 0.f };
		setRotation(rot);

		scp::f32vec3 dir = normalize(position - getTranslation());

		scp::f32vec3 dirPlane = { dir.x, 0.f, dir.z };
		if (scp::length(dirPlane) != 0.f)
		{
			const float angle = std::atan2(-dirPlane.x, -dirPlane.z);

			rotate(getUpVector(), angle);
		}

		dirPlane = { 0.f, dir.y, dir.z };
		if (scp::length(dirPlane) != 0.f)
		{
			const float angle = std::numbers::pi / 2 - std::acos(dot(dirPlane, { 0.f, 1.f, 0.f }));

			rotate(-getLeftVector(), angle);
		}

		if (dutchAngle != 0.f)
		{
			rotate(getFrontVector(), dutchAngle);
		}
	}

	constexpr scp::f32vec3 CameraBase::getUpVector() const
	{
		scp::f32vec3 result(0.f, 1.f, 0.f);
		applyRotationTo(result);
		return result;
	}

	constexpr scp::f32vec3 CameraBase::getFrontVector() const
	{
		scp::f32vec3 result(0.f, 0.f, -1.f);
		applyRotationTo(result);
		return result;
	}

	constexpr scp::f32vec3 CameraBase::getLeftVector() const
	{
		scp::f32vec3 result(-1.f, 0.f, 0.f);
		applyRotationTo(result);
		return result;
	}

	constexpr const scp::f32mat4x4& CameraBase::getViewMatrix() const
	{
		return getInverseTransformMatrix();
	}

	constexpr const scp::f32mat4x4& CameraBase::getInverseViewMatrix() const
	{
		return getTransformMatrix();
	}
}
