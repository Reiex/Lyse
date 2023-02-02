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

			constexpr CameraPerspective(float aspect, float near, float far, float fov);
			constexpr CameraPerspective(uint32_t width, uint32_t height, float near, float far, float fov);
			constexpr CameraPerspective(const CameraPerspective& camera) = default;
			constexpr CameraPerspective(CameraPerspective&& camera) = default;

			constexpr CameraPerspective& operator=(const CameraPerspective& camera) = default;
			constexpr CameraPerspective& operator=(CameraPerspective&& camera) = default;

			constexpr void setAspect(float aspect);
			constexpr void setAspect(uint32_t width, uint32_t height);
			constexpr void setNearDistance(float near);
			constexpr void setFarDistance(float far);
			constexpr void setFieldOfView(float fov);

			constexpr const scp::f32mat4x4& getProjectionMatrix() const;

			constexpr virtual ~CameraPerspective() override final = default;

		private:

			float _aspect;
			float _near;
			float _far;
			float _fov;

			mutable std::optional<scp::f32mat4x4> _projection;
	};
}
