///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreTypes.hpp>

namespace spl
{
	namespace ShaderStage
	{
		enum Stage
		{
			None					= 0,
			Compute					= 1 << 0,
			Vertex					= 1 << 1,
			TessControl				= 1 << 2,
			TessEvaluation			= 1 << 3,
			Geometry				= 1 << 4,
			Fragment				= 1 << 5
		};
	}

	class ShaderModule
	{
		public:

			ShaderModule();
			ShaderModule(ShaderStage::Stage stage, const std::filesystem::path& glslFile);
			ShaderModule(ShaderStage::Stage stage, const char* source, uint32_t size);
			ShaderModule(ShaderStage::Stage stage, const char* const* sources, const uint32_t* sizes, uint32_t count);
			ShaderModule(ShaderStage::Stage stage, const std::filesystem::path& spirvFile, const char* entryPoint, const uint32_t* constantIndices, const void* constantValues, uint32_t specializationConstantsCount);
			ShaderModule(ShaderStage::Stage stage, const void* binary, uint32_t size, const char* entryPoint, const uint32_t* constantIndices, const void* constantValues, uint32_t specializationConstantsCount);
			ShaderModule(const ShaderModule& shader) = delete;
			ShaderModule(ShaderModule&& shader) = delete;

			ShaderModule& operator=(const ShaderModule& shader) = delete;
			ShaderModule& operator=(ShaderModule&& shader) = delete;


			bool createFromGlsl(ShaderStage::Stage stage, const std::filesystem::path& glslFile);
			bool createFromGlsl(ShaderStage::Stage stage, const char* source, uint32_t size);
			bool createFromGlsl(ShaderStage::Stage stage, const char* const* sources, const uint32_t* sizes, uint32_t count);
			bool createFromSpirV(ShaderStage::Stage stage, const std::filesystem::path& spirvFile, const char* entryPoint, const uint32_t* constantIndices, const void* constantValues, uint32_t specializationConstantsCount);
			bool createFromSpirV(ShaderStage::Stage stage, const void* binary, uint32_t size, const char* entryPoint, const uint32_t* constantIndices, const void* constantValues, uint32_t specializationConstantsCount);
			// TODO: Handle binary with a new class "ShaderBinary"

			void destroy();


			uint32_t getHandle() const;
			bool isValid() const;


			~ShaderModule();

			static void releaseShaderCompiler();

		private:

			static bool _loadFile(const std::filesystem::path& filename, char*& data, uint32_t& size);

			uint32_t _shader;
			ShaderStage::Stage _stage;
			int32_t _compileStatus;
	};
}
