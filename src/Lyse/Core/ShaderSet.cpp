///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Lyse/Core/Core.hpp>

namespace lys
{
	namespace
	{
		const std::unordered_map<std::string, intptr_t> offsetsGBuffer = {
			{ "u_drawableColor",		offsetof(shaderInterface::InterfaceGBuffer, u_drawable.color) },
			{ "u_drawableMaterial",		offsetof(shaderInterface::InterfaceGBuffer, u_drawable.material) },
			{ "u_drawableShadowBias",	offsetof(shaderInterface::InterfaceGBuffer, u_drawable.shadowBias) },
			{ "u_drawableNormal",		offsetof(shaderInterface::InterfaceGBuffer, u_drawable.normal) }
		};

		const std::unordered_map<std::string, intptr_t> offsetsShadow = {
			{ "u_drawableColor",		offsetof(shaderInterface::InterfaceShadow, u_drawable.color) },
			{ "u_drawableMaterial",		offsetof(shaderInterface::InterfaceShadow, u_drawable.material) },
			{ "u_drawableShadowBias",	offsetof(shaderInterface::InterfaceShadow, u_drawable.shadowBias) },
			{ "u_drawableNormal",		offsetof(shaderInterface::InterfaceShadow, u_drawable.normal) },
			{ "u_gBufferDepth",			offsetof(shaderInterface::InterfaceShadow, u_gBuffer.depth) },
			{ "u_gBufferColor",			offsetof(shaderInterface::InterfaceShadow, u_gBuffer.color) },
			{ "u_gBufferMaterial",		offsetof(shaderInterface::InterfaceShadow, u_gBuffer.material) },
			{ "u_gBufferNormal",		offsetof(shaderInterface::InterfaceShadow, u_gBuffer.normal) },
			{ "u_gBufferTangent",		offsetof(shaderInterface::InterfaceShadow, u_gBuffer.tangent) }
		};

		const std::unordered_map<std::string, intptr_t> offsetsTransparency = {
			{ "u_drawableColor",		offsetof(shaderInterface::InterfaceTransparency, u_drawable.color) },
			{ "u_drawableMaterial",		offsetof(shaderInterface::InterfaceTransparency, u_drawable.material) },
			{ "u_drawableShadowBias",	offsetof(shaderInterface::InterfaceTransparency, u_drawable.shadowBias) },
			{ "u_drawableNormal",		offsetof(shaderInterface::InterfaceTransparency, u_drawable.normal) },
			{ "u_gBufferDepth",			offsetof(shaderInterface::InterfaceTransparency, u_gBuffer.depth) },
			{ "u_gBufferColor",			offsetof(shaderInterface::InterfaceTransparency, u_gBuffer.color) },
			{ "u_gBufferMaterial",		offsetof(shaderInterface::InterfaceTransparency, u_gBuffer.material) },
			{ "u_gBufferNormal",		offsetof(shaderInterface::InterfaceTransparency, u_gBuffer.normal) },
			{ "u_gBufferTangent",		offsetof(shaderInterface::InterfaceTransparency, u_gBuffer.tangent) },
			{ "u_shadowTexture",		offsetof(shaderInterface::InterfaceTransparency, u_shadow.texture) },
			{ "u_shadowBlurOffset",		offsetof(shaderInterface::InterfaceTransparency, u_shadow.blurOffset) }
		};
	}

	ShaderSet::ShaderSet(const spl::ShaderProgram* shaderGBuffer, const spl::ShaderProgram* shaderShadow, const spl::ShaderProgram* shaderTransparency) :
		_shaderGBuffer(shaderGBuffer),
		_shaderShadow(shaderShadow),
		_shaderTransparency(shaderTransparency),
		_interfaceGBuffer(),
		_interfaceShadow(),
		_interfaceTransparency()
	{
		fillInterface(_shaderGBuffer, offsetsGBuffer, &_interfaceGBuffer);
		fillInterface(_shaderShadow, offsetsShadow, &_interfaceShadow);
		fillInterface(_shaderTransparency, offsetsTransparency, &_interfaceTransparency);

		// TODO: Per-interface function to check: uniform types, uniform blocks, inputs and outputs (va/fo), etc...
	}

	void ShaderSet::fillInterface(const spl::ShaderProgram* shader, const std::unordered_map<std::string, intptr_t>& offsets, void* pInterface)
	{
		const spl::ShaderProgramInterfaceInfos& interfaceInfos = shader->getInterfaceInfos(spl::ShaderProgramInterface::Uniform);
		char* pcInterface = reinterpret_cast<char*>(pInterface);

		for (int32_t i = 0; i < interfaceInfos.activeResources; ++i)
		{
			const spl::ShaderProgramResourceInfos& resourceInfos = shader->getResourceInfos(spl::ShaderProgramInterface::Uniform, i);

			auto it = offsets.find(resourceInfos.name.c_str());
			if (it != offsets.end())
			{
				*reinterpret_cast<spl::GlslType*>(pcInterface + it->second) = resourceInfos.type;
			}
		}
	}
}
