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
	class CameraBase : public Transformable
	{
		public:

			// TODO: Make it constexpr when C++23 fully implemented in MSVC
			inline void lookAt(const scp::f32vec3& position, float dutchAngle = 0.f);


			virtual constexpr float getAspect() const = 0;
			virtual constexpr float getFieldOfView() const = 0;
			virtual constexpr float getNearDistance() const = 0;
			virtual constexpr float getFarDistance() const = 0;

			constexpr scp::f32vec3 getUpVector() const;
			constexpr scp::f32vec3 getFrontVector() const;
			constexpr scp::f32vec3 getLeftVector() const;

			constexpr const scp::f32mat4x4& getViewMatrix() const;
			constexpr const scp::f32mat4x4& getInverseViewMatrix() const;
			virtual constexpr const scp::f32mat4x4& getProjectionMatrix() const = 0;

		protected:

			constexpr CameraBase() = default;
			constexpr CameraBase(const CameraBase& camera) = default;
			constexpr CameraBase(CameraBase&& camera) = default;

			constexpr CameraBase& operator=(const CameraBase& camera) = default;
			constexpr CameraBase& operator=(CameraBase&& camera) = default;

			virtual constexpr ~CameraBase() override = default;
	};
}
