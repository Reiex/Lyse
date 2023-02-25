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

	class LightBase
	{
		public:

			constexpr void setCastShadows(bool castShadows);
			constexpr void setColor(float r, float g, float b);
			constexpr void setIntensity(float intensity);

			virtual constexpr LightType getType() const = 0;
			constexpr bool getCastShadows() const;
			constexpr const scp::f32vec3& getColor() const;
			constexpr float getIntensity() const;

		protected:

			constexpr LightBase(float r = 1.f, float g = 1.f, float b = 1.f, float intensity = 1.f);
			constexpr LightBase(const LightBase& light) = default;
			constexpr LightBase(LightBase&& light) = default;

			constexpr LightBase& operator=(const LightBase& light) = default;
			constexpr LightBase& operator=(LightBase&& light) = default;

			virtual constexpr void _getUboParams(const CameraBase* camera, scp::f32vec4* params) const = 0;
			virtual constexpr void _getShadowCameras(const CameraBase* camera, std::vector<const CameraBase*>& shadowCameras) const = 0;

			virtual constexpr ~LightBase() = default;

			bool _castShadows;
			scp::f32vec3 _color;
			float _intensity;

		friend class Scene;
	};
}
