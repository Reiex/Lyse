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
	enum class ShaderType
	{
		GBuffer,
		ShadowMapping
	};

	struct GBufferShaderInterface
	{
		spl::GlslType u_normalMap = spl::GlslType::Undefined;

		spl::GlslType u_color = spl::GlslType::Undefined;
		spl::GlslType u_material = spl::GlslType::Undefined;
	};

	struct ShadowMappingShaderInterface
	{
		// Nothing
	};

	class ShaderSet
	{
		public:

			ShaderSet(const spl::ShaderProgram* gBufferShader, const spl::ShaderProgram* shadowMappingShader);
			ShaderSet(const ShaderSet& set) = default;
			ShaderSet(ShaderSet&& set) = default;

			ShaderSet& operator=(const ShaderSet& set) = default;
			ShaderSet& operator=(ShaderSet&& set) = default;

			~ShaderSet() = default;

		private:

			static const std::unordered_map<std::string, intptr_t> gBufferUniformToOffset;

			const spl::ShaderProgram* _gBufferShader;
			const spl::ShaderProgram* _shadowMappingShader;

			GBufferShaderInterface _gBufferShaderInterface;
			ShadowMappingShaderInterface _shadowMappingShaderInterface;

		friend class Scene;
	};
}
