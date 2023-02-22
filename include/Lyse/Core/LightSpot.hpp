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

			constexpr LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut);
			constexpr LightSpot(float x, float y, float z, float xDir, float yDir, float zDir, float angularRadiusIn, float angularRadiusOut, float r, float g, float b, float intensity);
			constexpr LightSpot(const LightSpot& light) = default;
			constexpr LightSpot(LightSpot&& light) = default;

			constexpr LightSpot& operator=(const LightSpot& light) = default;
			constexpr LightSpot& operator=(LightSpot&& light) = default;

			
			using LightBase::setPosition;
			using LightBase::move;
			using LightBase::setDirection;
			using LightBase::rotate;
			constexpr void setAngularRadii(float in, float out);


			virtual constexpr LightType getType() const override final;
			using LightBase::getPosition;
			constexpr const scp::f32vec2& getAngularRadii() const;


			virtual constexpr ~LightSpot() override final = default;

		private:

			virtual inline void _getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const override final;

			scp::f32vec2 _radii;
	};
}
