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
	class CameraPerspective : public CameraBase
	{
		public:

			constexpr CameraPerspective(float aspect, float fov, float near, float far);
			constexpr CameraPerspective(uint32_t width, uint32_t height, float fov, float near, float far);
			constexpr CameraPerspective(const CameraPerspective& camera) = default;
			constexpr CameraPerspective(CameraPerspective&& camera) = default;

			constexpr CameraPerspective& operator=(const CameraPerspective& camera) = default;
			constexpr CameraPerspective& operator=(CameraPerspective&& camera) = default;

			constexpr void setAspect(float aspect);
			constexpr void setAspect(uint32_t width, uint32_t height);
			constexpr void setFieldOfView(float fov);
			constexpr void setNearDistance(float near);
			constexpr void setFarDistance(float far);

			virtual constexpr CameraType getType() const override final;

			virtual constexpr ~CameraPerspective() override final = default;

		private:

			// TODO: I have a dream ! That maths functions have a constexpr implementation !
			virtual inline void _updateProjectionMatrix() const override final;
			virtual inline void _updateInverseProjectionMatrix() const override final;
	};
}
