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

	ShaderSet::ShaderSet(const spl::ShaderProgram* gBufferShader) :
		_gBufferShader(gBufferShader),
		_gBufferShaderInterface()
	{
		// TODO: Check outputs

		char* shaderInterface = reinterpret_cast<char*>(&_gBufferShaderInterface);
		const spl::ShaderProgramInterfaceInfos* interfaceInfos = &gBufferShader->getInterfaceInfos(spl::ShaderProgramInterface::Uniform);

		for (int32_t i = 0; i < interfaceInfos->activeResources; ++i)
		{
			const spl::ShaderProgramResourceInfos& resourceInfos = gBufferShader->getResourceInfos(spl::ShaderProgramInterface::Uniform, i);

			auto it = gBufferUniformToOffset.find(resourceInfos.name.c_str());
			if (it != gBufferUniformToOffset.end())
			{
				*reinterpret_cast<spl::GlslType*>(shaderInterface + it->second) = resourceInfos.type;
			}
		}

		// TODO: Check uniforms and uniform blocks... (types, etc...)
	}
}
