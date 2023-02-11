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

			constexpr CameraBase();
			constexpr CameraBase(const CameraBase& camera) = default;
			constexpr CameraBase(CameraBase&& camera) = default;

			constexpr CameraBase& operator=(const CameraBase& camera) = default;
			constexpr CameraBase& operator=(CameraBase&& camera) = default;


			// TODO: Make it constexpr when C++23 fully implemented in MSVC
			inline void lookAt(const scp::f32vec3& position, float dutchAngle = 0.f);


			constexpr scp::f32vec3 getUpVector() const;
			constexpr scp::f32vec3 getFrontVector() const;
			constexpr scp::f32vec3 getLeftVector() const;

			constexpr virtual float getAspect() const = 0;
			constexpr virtual float getNearDistance() const = 0;
			constexpr virtual float getFarDistance() const = 0;
			constexpr const scp::f32mat4x4& getViewMatrix() const;
			constexpr virtual const scp::f32mat4x4& getProjectionMatrix() const = 0;


			constexpr virtual ~CameraBase() override = default;
	};
}
