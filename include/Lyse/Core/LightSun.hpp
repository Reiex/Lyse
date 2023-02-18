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

			constexpr LightSun(float xDir, float yDir, float zDir);
			constexpr LightSun(float xDir, float yDir, float zDir, float r, float g, float b, float intensity);
			constexpr LightSun(const LightSun& light) = default;
			constexpr LightSun(LightSun&& light) = default;

			constexpr LightSun& operator=(const LightSun& light) = default;
			constexpr LightSun& operator=(LightSun&& light) = default;

			
			constexpr void setLightDir(float x, float y, float z);


			virtual constexpr LightType getType() const override final;
			constexpr const scp::f32vec3& getLightDir() const;


			virtual constexpr ~LightSun() override final = default;

		private:

			virtual inline void _getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const override final;

			scp::f32vec3 _dir;
	};
}
