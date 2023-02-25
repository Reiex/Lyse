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
	class LightPoint : public LightBase
	{
		public:

			constexpr LightPoint(float x, float y, float z);
			constexpr LightPoint(float x, float y, float z, float r, float g, float b, float intensity);
			constexpr LightPoint(const LightPoint& light) = default;
			constexpr LightPoint(LightPoint&& light) = default;

			constexpr LightPoint& operator=(const LightPoint& light) = default;
			constexpr LightPoint& operator=(LightPoint&& light) = default;

			constexpr void setPosition(const scp::f32vec3& position);
			constexpr void setPosition(float x, float y, float z);
			constexpr void move(const scp::f32vec3& offset);
			constexpr void move(float dx, float dy, float dz);

			virtual constexpr LightType getType() const override final;
			constexpr const scp::f32vec3& getPosition() const;

			virtual constexpr ~LightPoint() override final = default;

		private:

			virtual constexpr void _getUboParams(const CameraBase* camera, scp::f32vec4* params) const override final;
			virtual constexpr void _getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const override final;

			scp::f32vec3 _position;
	};
}
