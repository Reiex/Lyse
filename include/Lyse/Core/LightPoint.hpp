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

			constexpr LightPoint(float xPos, float yPos, float zPos);
			constexpr LightPoint(float xPos, float yPos, float zPos, float r, float g, float b, float intensity);
			constexpr LightPoint(const LightPoint& light) = default;
			constexpr LightPoint(LightPoint&& light) = default;

			constexpr LightPoint& operator=(const LightPoint& light) = default;
			constexpr LightPoint& operator=(LightPoint&& light) = default;

			
			using LightBase::setPosition;
			using LightBase::move;


			virtual constexpr LightType getType() const override final;
			using LightBase::getPosition;


			virtual constexpr ~LightPoint() override final = default;

		private:

			virtual constexpr void _getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const override final;
	};
}
