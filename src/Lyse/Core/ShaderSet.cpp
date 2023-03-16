///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

namespace lys
{
	const std::unordered_map<std::string, intptr_t> ShaderSet::gBufferUniformToOffset = {
		{ "u_color",		offsetof(GBufferShaderInterface, u_color) },
		{ "u_material",		offsetof(GBufferShaderInterface, u_material) },
		{ "u_normalMap",	offsetof(GBufferShaderInterface, u_normalMap) }
	};

	const std::unordered_map<std::string, intptr_t> ShaderSet::shadowMappingUniformToOffset = {
		{ "u_color",		offsetof(ShadowMappingShaderInterface, u_color) },
		{ "u_depthBias",	offsetof(ShadowMappingShaderInterface, u_depthBias) }
	};

	ShaderSet::ShaderSet(const spl::ShaderProgram* gBufferShader, const spl::ShaderProgram* shadowMappingShader) :
		_gBufferShader(gBufferShader),
		_shadowMappingShader(shadowMappingShader),
		_gBufferShaderInterface(),
		_shadowMappingShaderInterface()
	{
		// TODO: Check outputs

		char* shaderInterface = reinterpret_cast<char*>(&_gBufferShaderInterface);
		const spl::ShaderProgramInterfaceInfos* interfaceInfos = &_gBufferShader->getInterfaceInfos(spl::ShaderProgramInterface::Uniform);

		for (int32_t i = 0; i < interfaceInfos->activeResources; ++i)
		{
			const spl::ShaderProgramResourceInfos& resourceInfos = _gBufferShader->getResourceInfos(spl::ShaderProgramInterface::Uniform, i);

			auto it = gBufferUniformToOffset.find(resourceInfos.name.c_str());
			if (it != gBufferUniformToOffset.end())
			{
				*reinterpret_cast<spl::GlslType*>(shaderInterface + it->second) = resourceInfos.type;
			}
		}


		shaderInterface = reinterpret_cast<char*>(&_shadowMappingShaderInterface);
		interfaceInfos = &_shadowMappingShader->getInterfaceInfos(spl::ShaderProgramInterface::Uniform);

		for (int32_t i = 0; i < interfaceInfos->activeResources; ++i)
		{
			const spl::ShaderProgramResourceInfos& resourceInfos = _shadowMappingShader->getResourceInfos(spl::ShaderProgramInterface::Uniform, i);

			auto it = shadowMappingUniformToOffset.find(resourceInfos.name.c_str());
			if (it != shadowMappingUniformToOffset.end())
			{
				*reinterpret_cast<spl::GlslType*>(shaderInterface + it->second) = resourceInfos.type;
			}
		}



		// TODO: Check uniforms and uniform blocks... (types, etc...)

		// TODO: One function per interface (gBuffer, shadowMapping... and do all of that for every interface)
	}
}
