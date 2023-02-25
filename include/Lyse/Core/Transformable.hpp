///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreTypes.hpp>

namespace lys
{
	class Transformable
	{
		public:

			constexpr Transformable();
			constexpr Transformable(const Transformable& transformable) = default;
			constexpr Transformable(Transformable&& transformable) = default;

			constexpr Transformable& operator=(const Transformable& transformable) = default;
			constexpr Transformable& operator=(Transformable&& transformable) = default;


			constexpr void setTranslation(const scp::f32vec3& translation);
			constexpr void setTranslation(float x, float y, float z);
			constexpr void setPosition(const scp::f32vec3& position);
			constexpr void setPosition(float x, float y, float z);
			constexpr void move(const scp::f32vec3& offset);
			constexpr void move(float dx, float dy, float dz);

			constexpr void setDirection(const scp::f32vec3& direction);
			constexpr void setDirection(float x, float y, float z);
			constexpr void setRotation(const scp::f32quat& rotation);
			constexpr void setRotation(const scp::f32vec3& axis, float angle);
			constexpr void setRotation(float xAxis, float yAxis, float zAxis, float angle);
			constexpr void rotate(const scp::f32quat& rotation);
			constexpr void rotate(const scp::f32vec3& axis, float angle);
			constexpr void rotate(float xAxis, float yAxis, float zAxis, float angle);

			constexpr void setScale(const scp::f32vec3& scale);
			constexpr void setScale(float xScale, float yScale, float zScale);
			constexpr void setScale(float scale);
			constexpr void scale(const scp::f32vec3& scale);
			constexpr void scale(float xScale, float yScale, float zScale);
			constexpr void scale(float scale);


			constexpr void applyTranslationTo(scp::f32vec3& vector) const;
			constexpr void applyTranslationTo(float& x, float& y, float& z) const;
			constexpr void applyRotationTo(scp::f32vec3& vector) const;
			constexpr void applyRotationTo(float& x, float& y, float& z) const;
			constexpr void applyScaleTo(scp::f32vec3& vector) const;
			constexpr void applyScaleTo(float& x, float& y, float& z) const;
			constexpr void applyTransformTo(scp::f32vec3& vector) const;
			constexpr void applyTransformTo(float& x, float& y, float& z) const;

			constexpr void applyInverseTranslationTo(scp::f32vec3& vector) const;
			constexpr void applyInverseTranslationTo(float& x, float& y, float& z) const;
			constexpr void applyInverseRotationTo(scp::f32vec3& vector) const;
			constexpr void applyInverseRotationTo(float& x, float& y, float& z) const;
			constexpr void applyInverseScaleTo(scp::f32vec3& vector) const;
			constexpr void applyInverseScaleTo(float& x, float& y, float& z) const;
			constexpr void applyInverseTransformTo(scp::f32vec3& vector) const;
			constexpr void applyInverseTransformTo(float& x, float& y, float& z) const;


			constexpr const scp::f32vec3& getTranslation() const;
			constexpr const scp::f32vec3& getPosition() const;
			constexpr const scp::f32quat& getRotation() const;
			constexpr const scp::f32vec3& getScale() const;
			
			virtual constexpr const scp::f32mat4x4& getTransformMatrix() const;
			constexpr const scp::f32mat4x4& getInverseTransformMatrix() const;


			constexpr virtual ~Transformable() = default;

		protected:

			scp::f32vec3 _translation;
			scp::f32quat _rotation;
			scp::f32vec3 _scale;
			
			mutable std::optional<scp::f32mat4x4> _transform;
			mutable std::optional<scp::f32mat4x4> _invTransform;
	};
}
