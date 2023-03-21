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
	enum class ContextReleaseBehaviour
	{
		None,
		Flush
	};

	enum class ProvokingVertexConvention
	{
		ProvokingVertex,
		FirstVertex,
		LastVertex,
		UndefinedVertex
	};

	namespace ContextFlags
	{
		enum Flags
		{
			None				= 0,
			ForwardCompatible	= 1 << 0,
			Debug				= 1 << 1,
			RobustAccess		= 1 << 2,
			NoError				= 1 << 3
		};
	}

	namespace ContextProfileMask
	{
		enum Flags
		{
			None					= 0,
			CoreProfile				= 1 << 0,
			CompatibilityProfile	= 1 << 1
		};
	}

	namespace ImplementationDependent
	{
		struct General
		{
			ContextReleaseBehaviour contextReleaseBehaviour;

			uint32_t maxClipDistances;
			uint32_t maxCullDistances;
			uint32_t maxCombinedClipAndCullDistances;

			uint32_t subpixelBits;

			uint64_t maxElementIndex;

			bool primitiveRestartForPatchesSupported;

			uint32_t max3dTextureSize;
			uint32_t maxTextureSize;
			uint32_t maxArrayTextureLayers;
			float maxTextureLodBias;
			uint32_t maxCubeMapTextureSize;
			uint32_t maxRenderbufferSize;
			float maxTextureMaxAnisotropy;

			scp::u32vec2 maxViewportDims;
			uint32_t maxViewports;
			uint32_t viewportSubpixelBits;
			scp::f32vec2 viewportBoundsRange;

			ProvokingVertexConvention layerProvokingVertex;
			ProvokingVertexConvention viewportIndexProvokingVertex;

			scp::f32vec2 pointSizeRange;
			float pointSizeGranularity;
			scp::f32vec2 aliasedLineWidthRange;
			scp::f32vec2 smoothLineWidthRange;
			float smoothLineWidthGranularity;

			uint32_t maxElementsIndices;
			uint32_t maxElementsVertices;

			uint32_t maxVertexAttribRelativeOffset;
			uint32_t maxVertexAttribBindings;
			uint32_t maxVertexAttribStride;

			std::vector<uint32_t> compressedTextureFormats;

			uint32_t maxTextureBufferSize;
			uint32_t maxRectangleTextureSize;

			std::vector<uint32_t> shaderBinaryFormats;
			std::vector<uint32_t> programBinaryFormats;
			bool shaderCompiler;

			uint32_t minMapBufferAlignment;
			uint32_t textureBufferOffsetAlignment;

			uint32_t maxFramebufferWidth;
			uint32_t maxFramebufferHeight;
			uint32_t maxFramebufferLayers;
			uint32_t maxFramebufferSamples;
			
			uint32_t maxSampleMaskWords;
			uint32_t maxSamples;
			uint32_t maxColorTextureSamples;
			uint32_t maxDepthTextureSamples;
			uint32_t maxIntegerSamples;

			uint64_t maxServerWaitTimeout;

			float minFragmentInterpolationOffset;
			float maxFragmentInterpolationOffset;
			uint32_t fragmentInterpolationOffsetBits;
			uint32_t maxDrawBuffers;
			uint32_t maxDualSourceDrawBuffers;
			uint32_t maxColorAttachments;
		};

		struct Version
		{
			uint32_t majorVersion;
			uint32_t minorVersion;
			ContextFlags::Flags contextFlags;
			ContextProfileMask::Flags contextProfile;
			std::vector<std::string> extensions;
			std::string renderer;
			std::vector<std::string> shadingLanguageVersions;
			std::vector<std::string> spirvExtensions;
			std::string vendor;
		};

		struct VertexShaderLimits
		{
			uint32_t maxVertexAttribs;
			uint32_t maxVertexUniformComponents;
			uint32_t maxVertexUniformVectors;
			uint32_t maxVertexUniformBlocks;
			uint32_t maxVertexOutputComponents;
			uint32_t maxVertexTextureImageUnits;
			uint32_t maxVertexAtomicCounterBuffers;
			uint32_t maxVertexAtomicCounters;
			uint32_t maxVertexShaderStorageBlocks;
		};

		struct TessellationShaderLimits
		{
			uint32_t maxTessGenLevel;
			uint32_t maxPatchVertices;

			uint32_t maxTessControlUniformComponents;
			uint32_t maxTessControlTextureImageUnits;
			uint32_t maxTessControlOutputComponents;
			uint32_t maxTessPatchComponents;
			uint32_t maxTessControlTotalOutputComponents;
			uint32_t maxTessControlInputComponents;
			uint32_t maxTessControlUniformBlocks;
			uint32_t maxTessControlAtomicCounterBuffers;
			uint32_t maxTessControlAotmicCounters;
			uint32_t maxTessControlShaderStorageBlocks;

			uint32_t maxTessEvaluationUniformComponents;
			uint32_t maxTessEvaluationTextureImageUnits;
			uint32_t maxTessEvaluationOutputComponents;
			uint32_t maxTessEvaluationInputComponents;
			uint32_t maxTessEvaluationUniformBlocks;
			uint32_t maxTessEvaluationAtomicCounterBuffers;
			uint32_t maxTessEvaluationAtomicCounters;
			uint32_t maxTessEvaluationShaderStorageBlocks;
		};

		struct GeometryShaderLimits
		{
			uint32_t maxGeometryUniformComponents;
			uint32_t maxGeometryUniformBlocks;
			uint32_t maxGeometryInputComponents;
			uint32_t maxGeometryOutputComponents;
			uint32_t maxGeometryOutputVertices;
			uint32_t maxGeometryTotalOutputComponents;
			uint32_t maxGeometryTextureImageUnits;
			uint32_t maxGeometryShaderInvocations;
			uint32_t maxVertexStreams;
			uint32_t maxGeometryAtomicCounterBuffers;
			uint32_t maxGeometryAtomicCounters;
			uint32_t maxGeometryShaderStorageBlocks;
		};

		struct FragmentShaderLimits
		{
			uint32_t maxFragmentUniformComponents;
			uint32_t maxFragmentUniformVectors;
			uint32_t maxFragmentUniformBlocks;
			uint32_t maxFragmentInputComponents;
			uint32_t maxTextureImageUnits;
			int32_t minProgramTextureGatherOffset;
			uint32_t maxProgramTextureGatherOffset;
			uint32_t maxFragmentAtomicCounterBuffers;
			uint32_t maxFragmentAtomicCounters;
			uint32_t maxFragmentShaderStorageBlocks;
		};

		struct ComputeShaderLimits
		{
			scp::u32vec3 maxComputeWorkGroupCount;
			scp::u32vec3 maxComputeWorkGroupSize;
			uint32_t maxComputeWorkGroupInvocations;
			uint32_t maxComputeUniformBlocks;
			uint32_t maxComputeTextureImageUnits;
			uint32_t maxComputeAtomicCounterBuffers;
			uint32_t maxComputeAtomicCounters;
			uint32_t maxComputeSharedMemorySize;
			uint32_t maxComputeUniformComponents;
			uint32_t maxComputeImageUniforms;
			uint32_t maxCombinedComputeUniformComponents;
			uint32_t maxComputeShaderStorageBlocks;
		};

		struct AggregateShaderLimits
		{
			int32_t minProgramTexelOffset;
			uint32_t maxProgramTexelOffset;
			uint32_t maxUniformBufferBindings;
			uint32_t maxUniformBlockSize;
			uint32_t uniformBufferOffsetAlignment;
			uint32_t maxCombinedUniformBlocks;
			uint32_t maxVaryingComponents;
			uint32_t maxVaryingVectors;
			uint32_t maxCombinedTextureImageUnits;
			uint32_t maxSubroutines;
			uint32_t maxSubroutineUniformLocations;
			uint32_t maxUniformLocations;
			uint32_t maxAtomicCounterBufferBindings;
			uint32_t maxAtomicCounterBufferSize;
			uint32_t maxCombinedAtomicCounterBuffers;
			uint32_t maxCombinedAtomicCounters;
			uint32_t maxShaderStorageBufferBindings;
			uint64_t maxShaderStorageBlockSize;
			uint32_t maxCombinedShaderStorageBlocks;
			uint32_t shaderStorageBufferOffsetAlignment;
			uint32_t maxImageUnits;
			uint32_t maxCombinedShaderOutputResources;
			uint32_t maxImageSamples;
			uint32_t maxVertexImageUniforms;
			uint32_t maxTessControlImageUniforms;
			uint32_t maxTessEvaluationImageUniforms;
			uint32_t maxGeometryImageUniforms;
			uint32_t maxFragmentImageUniforms;
			uint32_t maxCombinedImageUniforms;
			uint32_t maxCombinedVertexUniformComponents;
			uint32_t maxCombinedGeometryUniformComponents;
			uint32_t maxCombinedTessControlUniformComponents;
			uint32_t maxCombinedTessEvaluationUniformComponents;
			uint32_t maxCombinedFragmentUniformComponents;
		};

		struct Debug
		{
			uint32_t maxDebugMessageLength;
			uint32_t maxDebugLoggedMessages;
			uint32_t maxDebugGroupStackDepth;
			uint32_t maxLabelLength;
		};

		struct TransformFeedbackLimits
		{
			uint32_t maxTransformFeedbackInterleavedComponents;
			uint32_t maxTransformFeedbackSeparateAttribs;
			uint32_t maxTransformFeedbackSeparateComponents;
			uint32_t maxTransformFeedbackBuffers;
		};
	}

	struct ImplementationDependentValues
	{
		ImplementationDependent::General general;
		ImplementationDependent::Version version;
		ImplementationDependent::VertexShaderLimits vertexShader;
		ImplementationDependent::TessellationShaderLimits tessellationShader;
		ImplementationDependent::GeometryShaderLimits geometryShader;
		ImplementationDependent::FragmentShaderLimits fragmentShader;
		ImplementationDependent::ComputeShaderLimits computeShader;
		ImplementationDependent::AggregateShaderLimits shader;
		ImplementationDependent::Debug debug;
		ImplementationDependent::TransformFeedbackLimits transformFeedback;

		// TODO: internal format dependent values
	};

	enum class DebugMessageSource
	{
		Api,
		ShaderCompiler,
		WindowSystem,
		ThirdParty,
		Application,
		Other
	};

	enum class DebugMessageType
	{
		Error,
		DeprecatedBehavior,
		UndefinedBehavior,
		Performance,
		Portability,
		Marker,
		PushGroup,
		PopGroup,
		Other
	};

	enum class DebugMessageSeverity
	{
		High,
		Medium,
		Low,
		Notification
	};

	struct DebugMessage
	{
		DebugMessageSource source;
		DebugMessageType type;
		uint32_t id;

		DebugMessageSeverity severity;
		std::string descr;
	};

	enum class FaceOrientation
	{
		Front,
		Back,
		FrontAndBack
	};

	enum class FaceCullingMode
	{
		Disabled,
		ClockWise,
		CounterClockWise
	};

	enum class BlendEquation
	{
		Add,
		Substract,
		ReverseSubstract,
		Min,
		Max
	};

	enum class BlendFunc
	{
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		// TODO: Src1Color,
		// TODO: OneMinusSrc1Color,
		// TODO: Src1Alpha,
		// TODO: OneMinusSrc1Alpha
	};

	struct IndexedBufferBinding
	{
		const Buffer* buffer = nullptr;
		uintptr_t size = 4;
		uintptr_t offset = 0;
	};

	struct ContextState
	{
		bool isSeamlessCubeMapFilteringEnabled = false;
		scp::i32vec4 viewport = { 0, 0, 0, 0 };
		scp::f32vec4 clearColor = { 0.f, 0.f, 0.f, 0.f };
		double clearDepth = 1.0;
		int32_t clearStencil = 0;
		FaceCullingMode faceCullingMode = FaceCullingMode::Disabled;
		FaceOrientation faceCullingOrientation = FaceOrientation::Back;
		bool isDepthWriteEnabled = true;
		bool isDepthTestEnabled = false;
		CompareFunc depthTestFunc = CompareFunc::Less;
		bool isStencilTestEnabled = false;
		std::vector<bool> isBlendEnabled = {};
		std::vector<std::array<BlendEquation, 2>> blendEquations = {};
		std::vector<std::array<BlendFunc, 4>> blendFuncs = {};
		scp::f32vec4 blendColor = { 0.f, 0.f, 0.f, 0.f };

		// TODO: Pixel storage modes

		std::array<const Buffer*, 11> bufferBindings = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		std::array<std::vector<IndexedBufferBinding>, 4> indexedBufferBindings = {};
		std::vector<const Texture*> textureBindings = {};
		std::vector<const Sampler*> samplerBindings = {};
		std::array<const Framebuffer*, 2> framebufferBindings = { nullptr, nullptr };
		const ShaderProgram* shaderBinding = nullptr;

		static uint8_t bufferTargetToIndex(BufferTarget target);
		static uint8_t indexedBufferTargetToIndex(BufferTarget target);
		static uint8_t framebufferTargetToIndex(FramebufferTarget target);

		static BufferTarget indexToBufferTarget(uint8_t index);
		static BufferTarget indexToIndexedBufferTarget(uint8_t index);
		static FramebufferTarget indexToFramebufferTarget(uint8_t index);
	};

	class Context
	{
		public:

			Context(const Context& context) = delete;
			Context(Context&& context) = delete;

			Context& operator=(const Context& context) = delete;
			Context& operator=(Context&& context) = delete;

			// OpenGL implementation dependent parameters

			const ImplementationDependentValues& getImplementationDependentValues() const;

			// Debugging

			bool getIsDebugContext() const;
			bool pollDebugMessage(DebugMessage*& message);

			// OpenGL context state

			void setState(const ContextState& state);

			void setIsSeamlessCubeMapFilteringEnabled(bool isEnabled);
			void setViewport(int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height);
			void setClearColor(float r, float g, float b, float a);
			void setClearDepth(double clearDepth);
			void setClearStencil(int32_t clearStencil);
			void setFaceCullingMode(FaceCullingMode mode);
			void setFaceCullingOrientation(FaceOrientation orientation);
			void setIsDepthWriteEnabled(bool isEnabled);
			void setIsDepthTestEnabled(bool isEnabled);
			void setDepthTestFunc(CompareFunc func);
			void setIsStencilTestEnabled(bool isEnabled);
			// TODO: StencilFunc and StencilOp (be careful, multiple op and func depending on the face...)
			void setIsBlendEnabled(uint32_t drawBufferIndex, bool isEnabled);
			void setBlendEquations(uint32_t drawBufferIndex, BlendEquation colorEquation, BlendEquation alphaEquation);
			void setBlendFuncs(uint32_t drawBufferIndex, BlendFunc srcColorFunc, BlendFunc srcAlphaFunc, BlendFunc dstColorFunc, BlendFunc dstAlphaFunc);
			void setBlendColor(float r, float g, float b, float a);

			bool getIsSeamlessCubeMapFilteringEnabled() const;
			const scp::i32vec4& getViewport() const;
			const scp::f32vec4& getClearColor() const;
			double getClearDepth() const;
			int32_t getClearStencil() const;
			FaceCullingMode getFaceCullingMode() const;
			FaceOrientation getFaceCullingOrientation() const;
			bool getIsDepthWriteEnabled() const;
			bool getIsDepthTestEnabled() const;
			CompareFunc getDepthTestFunc() const;
			bool getIsStencilTestEnabled() const;
			// TODO: StencilFunc and StencilOp
			bool getIsBlendEnabled(uint32_t drawBufferIndex) const;
			const std::array<BlendEquation, 2>& getBlendEquations(uint32_t drawBufferIndex) const;
			const std::array<BlendFunc, 4>& getBlendFuncs(uint32_t drawBufferIndex) const;
			const scp::f32vec4& getBlendColor() const;

			const Buffer* getBufferBinding(BufferTarget target) const;
			const IndexedBufferBinding& getIndexedBufferBinding(BufferTarget target, uint32_t index) const;
			const Texture* getTextureBinding(uint32_t textureUnit) const;
			const Sampler* getSamplerBinding(uint32_t textureUnit) const;
			const Framebuffer* getFramebufferBinding(FramebufferTarget target) const;
			const ShaderProgram* getShaderBinding() const;

			const ContextState& getState() const;

			// Multi-context and window related functions

			Window* getWindow();

			static bool setCurrentContext(Context* context);

			static bool isInValidContext();
			static Context* getCurrentContext();

		private:

			static Context* _createContext();
			static bool _destroyContext(Context* context);

			Context();
			~Context();

			void _setWindow(Window* window);
			void _onFirstActivation();
			void _loadImplementationDependentValues();

			void _unbindBuffer(const Buffer* buffer);
			void _unbindTexture(const Texture* texture);
			void _unbindSampler(const Sampler* sampler);
			void _unbindFramebuffer(const Framebuffer* framebuffer);
			void _unbindShaderProgram(const ShaderProgram* program);

			ImplementationDependentValues _implementationDependentValues;

			bool _debugContext;
			std::queue<DebugMessage*> _debugMessages;
			DebugMessage* _lastDebugMessageSent;

			ContextState _state;

			Window* _window;
			bool _hasBeenActivated;

			static std::mutex _mutex;
			static std::unordered_set<Context*> _contexts;
			static std::unordered_map<std::thread::id, Context*> _currentContexts;

		friend class Window;
		friend void stackDebugMessage(DebugMessage* message, Context* context);

		friend class Buffer;
		friend class Sampler;
		friend class Texture;
		friend class Framebuffer;
		friend class ShaderProgram;
	};
}
