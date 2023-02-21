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
	class CameraOrthographic : public CameraBase
	{
		public:

			// fov != -1 means width and height only specifies an aspect and can be modified
			constexpr CameraOrthographic(float width, float height, float depth, float fov = -1.f);	
			constexpr CameraOrthographic(const CameraOrthographic& camera) = default;
			constexpr CameraOrthographic(CameraOrthographic&& camera) = default;

			constexpr CameraOrthographic& operator=(const CameraOrthographic& camera) = default;
			constexpr CameraOrthographic& operator=(CameraOrthographic&& camera) = default;

			constexpr void setAspect(float aspect);
			constexpr void setAspect(uint32_t width, uint32_t height);
			constexpr void setFieldOfView(float fov);
			constexpr void setDepth(float depth);

			virtual constexpr CameraType getType() const override final;
			virtual constexpr float getAspect() const override final;
			virtual constexpr float getFieldOfView() const override final;
			virtual constexpr float getNearDistance() const override final;
			virtual constexpr float getFarDistance() const override final;
			virtual constexpr const scp::f32mat4x4& getProjectionMatrix() const override final;

			virtual constexpr ~CameraOrthographic() override final = default;

		private:

			float _aspect;
			float _fov;
			float _depth;

			mutable std::optional<scp::f32mat4x4> _projection;
	};
}
