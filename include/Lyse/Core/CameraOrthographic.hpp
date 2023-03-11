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
			inline CameraOrthographic(float width = 1.f, float height = 1.f, float depth = 1.f);
			constexpr CameraOrthographic(const CameraOrthographic& camera) = default;
			constexpr CameraOrthographic(CameraOrthographic&& camera) = default;

			constexpr CameraOrthographic& operator=(const CameraOrthographic& camera) = default;
			constexpr CameraOrthographic& operator=(CameraOrthographic&& camera) = default;

			inline void setSize(float width, float height, float depth);

			virtual constexpr CameraType getType() const override final;

			virtual constexpr ~CameraOrthographic() override final = default;

		private:

			// TODO: I have a dream ! That maths functions have a constexpr implementation !
			virtual inline void _updateProjectionMatrix() const override final;
			virtual inline void _updateInverseProjectionMatrix() const override final;
	};
}
