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
			virtual constexpr float getAspect() const override final;
			virtual constexpr float getFieldOfView() const override final;
			virtual constexpr float getNearDistance() const override final;
			virtual constexpr float getFarDistance() const override final;
			virtual constexpr const scp::f32mat4x4& getProjectionMatrix() const override final;
			virtual constexpr const scp::f32mat4x4& getInverseProjectionMatrix() const override final;

			virtual constexpr ~CameraPerspective() override final = default;

		private:

			float _aspect;
			float _fov;
			float _near;
			float _far;

			mutable std::optional<scp::f32mat4x4> _projection;
			mutable std::optional<scp::f32mat4x4> _invProjection;
	};
}
