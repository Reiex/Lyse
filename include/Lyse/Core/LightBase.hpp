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
	enum class LightType
	{
		Point,
		Sun,
		Spot
	};

	class LightBase : public Transformable
	{
		public:

			constexpr void setColor(float r, float g, float b);
			constexpr void setIntensity(float intensity);

			virtual constexpr LightType getType() const = 0;
			constexpr const scp::f32vec3& getColor() const;
			constexpr float getIntensity() const;

		protected:

			constexpr LightBase(float r = 1.f, float g = 1.f, float b = 1.f, float intensity = 1.f);
			constexpr LightBase(const LightBase& light) = default;
			constexpr LightBase(LightBase&& light) = default;

			constexpr LightBase& operator=(const LightBase& light) = default;
			constexpr LightBase& operator=(LightBase&& light) = default;

			virtual constexpr void _getParams(const scp::f32mat4x4 view, scp::f32vec4* params) const = 0;

			virtual constexpr ~LightBase() override = default;

			scp::f32vec3 _color;
			float _intensity;

		friend class Scene;
	};
}
