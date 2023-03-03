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
	namespace ShaderProgramFlags
	{
		enum Flags
		{
			None				= 0,
			Separable			= 1 << 0,
			BinaryRetrievable	= 1 << 1
		};
	}

	enum class ShaderProgramInterface
	{
		Uniform,
		UniformBlock,
		AtomicCounterBuffer,
		ProgramInput,
		ProgramOutput,
		TransformFeedbackVarying,
		TransformFeedbackBuffer,
		BufferVariable,
		ShaderStorageBlock,

		ComputeSubroutine,
		VertexSubroutine,
		TessControlSubroutine,
		TessEvaluationSubroutine,
		GeometrySubroutine,
		FragmentSubroutine,

		ComputeSubroutineUniform,
		VertexSubroutineUniform,
		TessControlSubroutineUniform,
		TessEvaluationSubroutineUniform,
		GeometrySubroutineUniform,
		FragmentSubroutineUniform
	};

	enum class GlslType
	{
		Undefined,

		Float,
		FloatVec2,
		FloatVec3,
		FloatVec4,
		Double,
		DoubleVec2,
		DoubleVec3,
		DoubleVec4,
		Int,
		IntVec2,
		IntVec3,
		IntVec4,
		UnsignedInt,
		UnsignedIntVec2,
		UnsignedIntVec3,
		UnsignedIntVec4,
		Bool,
		BoolVec2,
		BoolVec3,
		BoolVec4,

		FloatMat2x2,
		FloatMat2x3,
		FloatMat2x4,
		FloatMat3x2,
		FloatMat3x3,
		FloatMat3x4,
		FloatMat4x2,
		FloatMat4x3,
		FloatMat4x4,
		DoubleMat2x2,
		DoubleMat2x3,
		DoubleMat2x4,
		DoubleMat3x2,
		DoubleMat3x3,
		DoubleMat3x4,
		DoubleMat4x2,
		DoubleMat4x3,
		DoubleMat4x4,

		Sampler1d,
		Sampler2d,
		Sampler3d,
		SamplerCube,
		Sampler1dShadow,
		Sampler2dShadow,
		SamplerCubeShadow,
		Sampler1dArray,
		Sampler2dArray,
		SamplerCubeArray,
		Sampler1dArrayShadow,
		Sampler2dArrayShadow,
		SamplerCubeArrayShadow,
		Sampler2dMultisample,
		Sampler2dMultisampleArray,
		SamplerBuffer,
		Sampler2dRect,
		Sampler2dRectShadow,
		IntSampler1d,
		IntSampler2d,
		IntSampler3d,
		IntSamplerCube,
		IntSampler1dArray,
		IntSampler2dArray,
		IntSamplerCubeArray,
		IntSampler2dMultisample,
		IntSampler2dMultisampleArray,
		IntSamplerBuffer,
		IntSampler2dRect,
		UnsignedIntSampler1d,
		UnsignedIntSampler2d,
		UnsignedIntSampler3d,
		UnsignedIntSamplerCube,
		UnsignedIntSampler1dArray,
		UnsignedIntSampler2dArray,
		UnsignedIntSamplerCubeArray,
		UnsignedIntSampler2dMultisample,
		UnsignedIntSampler2dMultisampleArray,
		UnsignedIntSamplerBuffer,
		UnsignedIntSampler2dRect,
		
		Image1d,
		Image2d,
		Image3d,
		Image2dRect,
		ImageCube,
		ImageBuffer,
		Image1dArray,
		Image2dArray,
		ImageCubeArray,
		Image2dMultisample,
		Image2dMultisampleArray,
		IntImage1d,
		IntImage2d,
		IntImage3d,
		IntImage2dRect,
		IntImageCube,
		IntImageBuffer,
		IntImage1dArray,
		IntImage2dArray,
		IntImageCubeArray,
		IntImage2dMultisample,
		IntImage2dMultisampleArray,
		UnsignedIntImage1d,
		UnsignedIntImage2d,
		UnsignedIntImage3d,
		UnsignedIntImage2dRect,
		UnsignedIntImageCube,
		UnsignedIntImageBuffer,
		UnsignedIntImage1dArray,
		UnsignedIntImage2dArray,
		UnsignedIntImageCubeArray,
		UnsignedIntImage2dMultisample,
		UnsignedIntImage2dMultisampleArray,

		UnsignedIntAtomicCounter
	};

	struct ShaderProgramInterfaceInfos
	{
		int32_t activeResources = 0;
		int32_t maxNameLength = 0;
		int32_t maxNumActiveVariables = 0;
		int32_t maxNumCompatibleSubroutines = 0;
	};

	struct ShaderProgramResourceInfos
	{
		std::string name = "";
		GlslType type = GlslType::Undefined;
		ShaderStage::Stage referencedBy = ShaderStage::None;

		uint32_t arraySize = 0;
		uint32_t arrayStride = -1;

		bool isRowMajor = false;
		uint32_t matrixStride = -1;

		uint32_t bufferBinding = -1;
		uint32_t blockIndex = -1;
		uint32_t atomicCounterBufferIndex = -1;
		uint32_t offset = -1;

		uint32_t topLevelArraySize = 0;
		uint32_t topLevelArrayStride = -1;

		uint32_t transformFeedbackBufferIndex = -1;
		uint32_t transformFeedbackBufferStride = -1;

		std::vector<uint32_t> activeVariables = {};
		std::vector<uint32_t> compatibleSubroutines = {};
		uint32_t bufferDataSize = 0;
		uint32_t locationComponent = 0;
		bool isPerPatch = false;
	};

	class ShaderProgram
	{
		public:

			// TODO: Give the possibility to not retrieve all the shader resource/interface informations directly !
			ShaderProgram();
			ShaderProgram(const std::filesystem::path& glslCompute, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const std::filesystem::path& glslVertex, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const std::filesystem::path& glslVertex, const std::filesystem::path& glslGeometry, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const std::filesystem::path& glslVertex, const std::filesystem::path& glslTessControl, const std::filesystem::path& glslTessEval, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const std::filesystem::path& glslVertex, const std::filesystem::path& glslTessControl, const std::filesystem::path& glslTessEval, const std::filesystem::path& glslGeometry, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const ShaderModule* const* shaders, uint8_t count, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			ShaderProgram(const ShaderProgram& program) = delete;
			ShaderProgram(ShaderProgram&& program) = delete;

			ShaderProgram& operator=(const ShaderProgram& program) = delete;
			ShaderProgram& operator=(ShaderProgram&& program) = delete;


			bool createFromGlsl(const std::filesystem::path& glslCompute, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			bool createFromGlsl(const std::filesystem::path& glslVertex, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			bool createFromGlsl(const std::filesystem::path& glslVertex, const std::filesystem::path& glslGeometry, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			bool createFromGlsl(const std::filesystem::path& glslVertex, const std::filesystem::path& glslTessControl, const std::filesystem::path& glslTessEval, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			bool createFromGlsl(const std::filesystem::path& glslVertex, const std::filesystem::path& glslTessControl, const std::filesystem::path& glslTessEval, const std::filesystem::path& glslGeometry, const std::filesystem::path& glslFragment, ShaderProgramFlags::Flags flags = ShaderProgramFlags::None);
			bool createFromShaderModules(const ShaderModule* const* shaders, uint16_t count, ShaderProgramFlags::Flags = ShaderProgramFlags::None);
			// TODO: Handle binary with a new class "ShaderBinary"
			// TODO: Handle pipelines with a new class "ShaderPipeline"

			void destroy();


			const ShaderProgramInterfaceInfos& getInterfaceInfos(ShaderProgramInterface programInterface) const;
			const ShaderProgramResourceInfos& getResourceInfos(ShaderProgramInterface programInterface, uint32_t index) const;


			template<CGlslScalarType TScalar> void setUniform(const std::string& name, const TScalar& scalar) const;
			template<CGlslScalarType TScalar> void setUniform(const std::string& name, const TScalar* scalars, uint32_t count) const;
			template<CGlslVecType TVec> void setUniform(const std::string& name, const TVec& vec) const;
			template<CGlslVecType TVec> void setUniform(const std::string& name, const TVec* vecs, uint32_t count) const;
			template<CGlslMatType TMat> void setUniform(const std::string& name, const TMat& mat) const;
			template<CGlslMatType TMat> void setUniform(const std::string& name, const TMat* mats, uint32_t count) const;
			void setUniform(const std::string& name, uint32_t textureUnit, const Texture* texture) const;

			void setUniformBlockBinding(uint32_t shaderBindingIndex, uint32_t bufferBindingIndex) const;
			void setShaderStorageBlockBinding(uint32_t shaderBindingIndex, uint32_t bufferBindingIndex) const;


			uint32_t getHandle() const;
			ShaderProgramFlags::Flags getFlags() const;
			bool isValid() const;


			~ShaderProgram();


			static void bind(const ShaderProgram* program);

		private:

			void _shaderIntrospection();
			void _setUniform(const std::string& name, GlslType type, const void* values, uint32_t count) const;

			static constexpr uint8_t _interfaceCount = 21;

			uint32_t _program;
			ShaderProgramFlags::Flags _flags;
			int32_t _linkStatus;

			std::array<ShaderProgramInterfaceInfos, _interfaceCount> _interfacesInfos;
			std::array<std::vector<ShaderProgramResourceInfos>, _interfaceCount> _resourcesInfos;

			std::array<std::unordered_map<std::string, uint32_t>, _interfaceCount> _locations;
			std::array<std::unordered_map<std::string, uint32_t>, _interfaceCount> _locationIndices;
	};
}
