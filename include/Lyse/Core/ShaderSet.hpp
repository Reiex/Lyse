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
		ShadowMapping,
		Transparency
	};

	namespace shaderInterface
	{
		struct SubInterfaceDrawable
		{
			spl::GlslType color = spl::GlslType::Undefined;
			spl::GlslType material = spl::GlslType::Undefined;
			spl::GlslType shadowBias = spl::GlslType::Undefined;

			spl::GlslType normal = spl::GlslType::Undefined;
		};

		struct SubInterfaceGBufferResult
		{
			spl::GlslType depth = spl::GlslType::Undefined;
			spl::GlslType color = spl::GlslType::Undefined;
			spl::GlslType material = spl::GlslType::Undefined;
			spl::GlslType normal = spl::GlslType::Undefined;
			spl::GlslType tangent = spl::GlslType::Undefined;
		};

		struct SubInterfaceShadowResult
		{
			spl::GlslType texture = spl::GlslType::Undefined;
			spl::GlslType offset = spl::GlslType::Undefined;
		};

		struct SubInterfaceTransparencyResult
		{
			spl::GlslType color = spl::GlslType::Undefined;
			spl::GlslType counter = spl::GlslType::Undefined;
		};

		struct SubInterfaceSsaoResult
		{
			spl::GlslType texture = spl::GlslType::Undefined;
		};

		struct InterfaceGBuffer
		{
			SubInterfaceDrawable u_drawable = {};
		};

		struct InterfaceShadow
		{
			SubInterfaceDrawable u_drawable = {};
			SubInterfaceGBufferResult u_gBuffer = {};
		};

		struct InterfaceTransparency
		{
			SubInterfaceDrawable u_drawable = {};
			SubInterfaceGBufferResult u_gBuffer = {};
			SubInterfaceShadowResult u_shadow = {};
		};
	}

	class ShaderSet
	{
		public:

			ShaderSet(const spl::ShaderProgram* shaderGBuffer, const spl::ShaderProgram* shaderShadow, const spl::ShaderProgram* shaderTransparency);
			ShaderSet(const ShaderSet& set) = default;
			ShaderSet(ShaderSet&& set) = default;

			ShaderSet& operator=(const ShaderSet& set) = default;
			ShaderSet& operator=(ShaderSet&& set) = default;

			~ShaderSet() = default;

		private:

			static void fillInterface(const spl::ShaderProgram* shader, const std::unordered_map<std::string, intptr_t>& offsets, void* pInterface);

			const spl::ShaderProgram* _shaderGBuffer;
			const spl::ShaderProgram* _shaderShadow;
			const spl::ShaderProgram* _shaderTransparency;

			shaderInterface::InterfaceGBuffer _interfaceGBuffer;
			shaderInterface::InterfaceShadow _interfaceShadow;
			shaderInterface::InterfaceTransparency _interfaceTransparency;

		friend class Scene;
	};
}
