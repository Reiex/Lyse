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
	class Material
	{
		public:

			Material() = default;
			Material(float red, float green, float blue, float ambiant, float metallic, float roughness);
			Material(const Material& material) = default;
			Material(Material&& material) = default;

			Material& operator=(const Material& material) = default;
			Material& operator=(Material&& material) = default;

			~Material() = default;

		// private:

			scp::f32vec3 _color;
			scp::f32vec3 _props;
	};
}
