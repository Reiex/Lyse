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
	class LightSun : public LightBase
	{
		public:

			inline LightSun(float xDir, float yDir, float zDir);
			inline LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity);
			constexpr LightSun(const LightSun& light) = default;
			constexpr LightSun(LightSun&& light) = default;

			constexpr LightSun& operator=(const LightSun& light) = default;
			constexpr LightSun& operator=(LightSun&& light) = default;

			constexpr const void setDirection(const scp::f32vec3& direction);
			constexpr const void setDirection(float x, float y, float z);

			virtual constexpr LightType getType() const override final;
			constexpr const scp::f32vec3& getDirection() const;

			virtual constexpr ~LightSun() override final = default;

		private:

			virtual constexpr void _getUboParams(const CameraBase* camera, scp::f32vec4* params) const override final;
			virtual inline void _getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const override final;

			mutable CameraOrthographic _camera;
	};
}
