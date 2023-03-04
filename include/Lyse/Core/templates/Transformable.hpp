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
	constexpr Transformable::Transformable() :
		_translation(0.f, 0.f, 0.f),
		_rotation(1.f, 0.f, 0.f, 0.f),
		_scale(1.f, 1.f, 1.f),
		_transform(),
		_invTransform()
	{
	}

	constexpr void Transformable::setTranslation(const scp::f32vec3& translation)
	{
		_translation = translation;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setTranslation(float x, float y, float z)
	{
		_translation.x = x;
		_translation.y = y;
		_translation.z = z;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setPosition(const scp::f32vec3& position)
	{
		_translation = position;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setPosition(float x, float y, float z)
	{
		_translation.x = x;
		_translation.y = y;
		_translation.z = z;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::move(const scp::f32vec3& offset)
	{
		_translation += offset;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::move(float dx, float dy, float dz)
	{
		_translation.x += dx;
		_translation.y += dy;
		_translation.z += dz;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setDirection(const scp::f32vec3& direction, float dutchAngle)
	{
		_rotation.setFromUnitVectorRotation(0.f, 0.f, -1.f, direction.x, direction.y, direction.z, dutchAngle);

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setDirection(float x, float y, float z, float dutchAngle)
	{
		_rotation.setFromUnitVectorRotation(0.f, 0.f, -1.f, x, y, z, dutchAngle);

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setRotation(const scp::f32quat& rotation)
	{
		// TODO: Uncomment this when C++23 fully implemented in MSVC
		// assert(std::abs(rotation.normSq() - 1.f) < 1e-2f);

		_rotation = rotation;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setRotation(const scp::f32vec3& axis, float angle)
	{
		_rotation.setFromRotationAxisAngle(axis.x, axis.y, axis.z, angle);

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setRotation(float xAxis, float yAxis, float zAxis, float angle)
	{
		_rotation.setFromRotationAxisAngle(xAxis, yAxis, zAxis, angle);

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::rotate(const scp::f32quat& rotation)
	{
		// TODO: Uncomment this when C++23 fully implemented in MSVC
		// assert(std::abs(rotation.normSq() - 1.f) < 1e-2f);

		_rotation = rotation * _rotation;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::rotate(const scp::f32vec3& axis, float angle)
	{
		scp::f32quat rotation;
		rotation.setFromRotationAxisAngle(axis.x, axis.y, axis.z, angle);

		rotate(rotation);
	}

	constexpr void Transformable::rotate(float xAxis, float yAxis, float zAxis, float angle)
	{
		scp::f32quat rotation;
		rotation.setFromRotationAxisAngle(xAxis, yAxis, zAxis, angle);

		rotate(rotation);
	}

	constexpr void Transformable::setScale(const scp::f32vec3& scale)
	{
		_scale = scale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setScale(float xScale, float yScale, float zScale)
	{
		_scale.x = xScale;
		_scale.y = yScale;
		_scale.z = zScale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::setScale(float scale)
	{
		_scale.x = scale;
		_scale.y = scale;
		_scale.z = scale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::scale(const scp::f32vec3& scale)
	{
		_scale *= scale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::scale(float xScale, float yScale, float zScale)
	{
		_scale.x *= xScale;
		_scale.y *= yScale;
		_scale.z *= zScale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::scale(float scale)
	{
		_scale.x *= scale;
		_scale.y *= scale;
		_scale.z *= scale;

		_transform.reset();
		_invTransform.reset();
	}

	constexpr void Transformable::applyTranslationTo(scp::f32vec3& vector) const
	{
		vector += _translation;
	}

	constexpr void Transformable::applyTranslationTo(float& x, float& y, float& z) const
	{
		x += _translation.x;
		y += _translation.y;
		z += _translation.z;
	}

	constexpr void Transformable::applyRotationTo(scp::f32vec3& vector) const
	{
		scp::f32quat v(0.f, vector.x, vector.y, vector.z);

		scp::f32quat inv = _rotation;
		inv.conjugate();

		v = _rotation * v * inv;

		vector.x = v.x;
		vector.y = v.y;
		vector.z = v.z;
	}

	constexpr void Transformable::applyRotationTo(float& x, float& y, float& z) const
	{
		scp::f32quat v(0.f, x, y, z);

		scp::f32quat inv = _rotation;
		inv.conjugate();

		v = _rotation * v * inv;

		x = v.x;
		y = v.y;
		z = v.z;
	}

	constexpr void Transformable::applyScaleTo(scp::f32vec3& vector) const
	{
		vector *= _scale;
	}

	constexpr void Transformable::applyScaleTo(float& x, float& y, float& z) const
	{
		x *= _scale.x;
		y *= _scale.y;
		z *= _scale.z;
	}

	constexpr void Transformable::applyTransformTo(scp::f32vec3& vector) const
	{
		applyScaleTo(vector);
		applyRotationTo(vector);
		applyTranslationTo(vector);
	}

	constexpr void Transformable::applyTransformTo(float& x, float& y, float& z) const
	{
		applyScaleTo(x, y, z);
		applyRotationTo(x, y, z);
		applyTranslationTo(x, y, z);
	}

	constexpr void Transformable::applyInverseTranslationTo(scp::f32vec3& vector) const
	{
		vector -= _translation;
	}

	constexpr void Transformable::applyInverseTranslationTo(float& x, float& y, float& z) const
	{
		x -= _translation.x;
		y -= _translation.y;
		z -= _translation.z;
	}

	constexpr void Transformable::applyInverseRotationTo(scp::f32vec3& vector) const
	{
		scp::f32quat v(0.f, vector.x, vector.y, vector.z);

		scp::f32quat inv = _rotation;
		inv.inverse();

		v = inv * v * _rotation;

		vector.x = v.x;
		vector.y = v.y;
		vector.z = v.z;
	}

	constexpr void Transformable::applyInverseRotationTo(float& x, float& y, float& z) const
	{
		scp::f32quat v(0.f, x, y, z);

		scp::f32quat inv = _rotation;
		inv.inverse();

		v = inv * v * _rotation;

		x = v.x;
		y = v.y;
		z = v.z;
	}

	constexpr void Transformable::applyInverseScaleTo(scp::f32vec3& vector) const
	{
		vector *= _scale;
	}

	constexpr void Transformable::applyInverseScaleTo(float& x, float& y, float& z) const
	{
		x /= _scale.x;
		y /= _scale.y;
		z /= _scale.z;
	}

	constexpr void Transformable::applyInverseTransformTo(scp::f32vec3& vector) const
	{
		applyInverseTranslationTo(vector);
		applyInverseRotationTo(vector);
		applyInverseScaleTo(vector);
	}

	constexpr void Transformable::applyInverseTransformTo(float& x, float& y, float& z) const
	{
		applyInverseTranslationTo(x, y, z);
		applyInverseRotationTo(x, y, z);
		applyInverseScaleTo(x, y, z);
	}

	constexpr const scp::f32vec3& Transformable::getTranslation() const
	{
		return _translation;
	}

	constexpr const scp::f32vec3& Transformable::getPosition() const
	{
		return _translation;
	}

	constexpr const scp::Quat<float>& Transformable::getRotation() const
	{
		return _rotation;
	}

	constexpr const scp::f32vec3& Transformable::getScale() const
	{
		return _scale;
	}
	
	constexpr const scp::f32mat4x4& Transformable::getTransformMatrix() const
	{
		if (!_transform.has_value())
		{
			const float& tx = _translation.x;
			const float& ty = _translation.y;
			const float& tz = _translation.z;

			const float aa = _rotation.w * _rotation.w;
			const float bb = _rotation.x * _rotation.x;
			const float cc = _rotation.y * _rotation.y;
			const float dd = _rotation.z * _rotation.z;

			const float ab2 = 2.f * _rotation.w * _rotation.x;
			const float ac2 = 2.f * _rotation.w * _rotation.y;
			const float ad2 = 2.f * _rotation.w * _rotation.z;

			const float bc2 = 2.f * _rotation.x * _rotation.y;
			const float bd2 = 2.f * _rotation.x * _rotation.z;

			const float cd2 = 2.f * _rotation.y * _rotation.z;

			const float r11 = aa + bb - cc - dd;
			const float r12 = bc2 - ad2;
			const float r13 = ac2 + bd2;
			const float r21 = ad2 + bc2;
			const float r22 = aa - bb + cc - dd;
			const float r23 = cd2 - ab2;
			const float r31 = bd2 - ac2;
			const float r32 = ab2 + cd2;
			const float r33 = aa - bb - cc + dd;

			const float& sx = _scale.x;
			const float& sy = _scale.y;
			const float& sz = _scale.z;

			_transform.emplace({
				r11 * sx, r12 * sy, r13 * sz, tx,
				r21 * sx, r22 * sy, r23 * sz, ty,
				r31 * sx, r32 * sy, r33 * sz, tz,
				0.f     , 0.f     , 0.f     , 1.f
				});
		}

		return *_transform;
	}

	constexpr const scp::f32mat4x4& Transformable::getInverseTransformMatrix() const
	{
		if (!_invTransform.has_value())
		{
			const float sx = 1.f / _scale.x;
			const float sy = 1.f / _scale.y;
			const float sz = 1.f / _scale.z;

			scp::Quat<float> rot = _rotation;
			rot.inverse();

			const float aa = rot.w * rot.w;
			const float bb = rot.x * rot.x;
			const float cc = rot.y * rot.y;
			const float dd = rot.z * rot.z;

			const float ab2 = 2.f * rot.w * rot.x;
			const float ac2 = 2.f * rot.w * rot.y;
			const float ad2 = 2.f * rot.w * rot.z;

			const float bc2 = 2.f * rot.x * rot.y;
			const float bd2 = 2.f * rot.x * rot.z;

			const float cd2 = 2.f * rot.y * rot.z;

			const float r11 = sx * (aa + bb - cc - dd);
			const float r12 = sx * (bc2 - ad2);
			const float r13 = sx * (ac2 + bd2);
			const float r21 = sy * (ad2 + bc2);
			const float r22 = sy * (aa - bb + cc - dd);
			const float r23 = sy * (cd2 - ab2);
			const float r31 = sz * (bd2 - ac2);
			const float r32 = sz * (ab2 + cd2);
			const float r33 = sz * (aa - bb - cc + dd);

			const float tx = -_translation.x;
			const float ty = -_translation.y;
			const float tz = -_translation.z;

			_invTransform.emplace({
				r11, r12, r13, r11 * tx + r12 * ty + r13 * tz,
				r21, r22, r23, r21 * tx + r22 * ty + r23 * tz,
				r31, r32, r33, r31 * tx + r32 * ty + r33 * tz,
				0.f, 0.f, 0.f, 1.f
				});
		}

		return *_invTransform;
	}
}
