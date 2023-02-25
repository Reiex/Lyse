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
	class LightSpot : public LightBase
	{
		public:
	
			constexpr LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angleIn, float angleOut);
			constexpr LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angleIn, float angleOut, float r, float g, float b, float intensity);
			constexpr LightSpot(const LightSpot& light) = default;
			constexpr LightSpot(LightSpot&& light) = default;
	
			constexpr LightSpot& operator=(const LightSpot& light) = default;
			constexpr LightSpot& operator=(LightSpot&& light) = default;
	
			constexpr void setPosition(const scp::f32vec3& position);
			constexpr void setPosition(float x, float y, float z);
			constexpr void move(const scp::f32vec3& offset);
			constexpr void move(float dx, float dy, float dz);
			constexpr const void setDirection(const scp::f32vec3& direction);
			constexpr const void setDirection(float x, float y, float z);
			constexpr void setAngularRadii(float in, float out);
	
			virtual constexpr LightType getType() const override final;
			constexpr const scp::f32vec3& getPosition() const;
			constexpr const scp::f32vec3& getDirection() const;
			constexpr float getAngleIn() const;
			constexpr float getAngleOut() const;
	
	
			virtual constexpr ~LightSpot() override final = default;
	
		private:
	
			virtual inline void _getUboParams(const CameraBase* camera, scp::f32vec4* params) const override final;
			virtual constexpr void _getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const override final;
	
			CameraPerspective _camera;
			float _angleIn;
	};
}
