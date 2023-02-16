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

			Material();
			Material(float red, float green, float blue, float alpha, float ambiant, float metallic, float roughness);
			Material(const Material& material) = default;
			Material(Material&& material) = default;

			Material& operator=(const Material& material) = default;
			Material& operator=(Material&& material) = default;

			void setColor(float red, float green, float blue, float alpha);
			void setProperties(float ambiant, float metallic, float roughness);
			void setColorTexture(const spl::Texture2D* texture);
			void setPropertiesTexture(const spl::Texture2D* texture);

			const scp::f32vec4& getColor() const;
			const scp::f32vec3& getProperties() const;
			const spl::Texture2D* getColorTexture() const;
			const spl::Texture2D* getPropertiesTexture() const;

			~Material() = default;

			static const Material defaultMaterial;

		private:

			scp::f32vec4 _color;
			scp::f32vec3 _props;

			const spl::Texture2D* _colorTexture;
			const spl::Texture2D* _propsTexture;
	};
}
