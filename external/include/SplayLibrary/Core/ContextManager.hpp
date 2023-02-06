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

	class Context
	{
		public:

			Context(const Context& context) = delete;
			Context(Context&& context) = delete;

			Context& operator=(const Context& context) = delete;
			Context& operator=(Context&& context) = delete;


			void setClearColor(float r, float g, float b, float a);
			void setClearDepth(double clearDepth);
			void setClearStencil(int32_t clearStencil);
			void setViewport(int32_t xOffset, int32_t yOffset, uint32_t width, uint32_t height);
			void setIsDepthTestEnabled(bool isEnabled);


			bool pollDebugMessage(DebugMessage*& message);


			Window* getWindow();
			bool getIsDebugContext() const;

			const Buffer* getBufferBinding(BufferTarget target, uint32_t index = -1) const;
			const RawTexture* getTextureBinding(TextureTarget target, uint32_t textureUnit) const;
			const Framebuffer* getFramebufferBinding(FramebufferTarget target) const;
			const ShaderProgram* getShaderBinding() const;

			const scp::f32vec4& getClearColor() const;
			double getClearDepth() const;
			int32_t getClearStencil() const;
			const scp::i32vec2& getViewportOffset() const;
			const scp::u32vec2& getViewportSize() const;
			bool getIsDepthTestEnabled() const;


			~Context();

		private:

			Context();

			void _setWindow(Window* window);
			void _onFirstActivation();

			Window* _window;
			bool _debugContext;
			bool _hasBeenActivated;

			std::array<const Buffer*, 11> _bufferBindings;
			std::array<std::vector<const Buffer*>, 4> _indexedBufferBindings;
			std::vector<std::array<const RawTexture*, 11>> _textureBindings;
			std::array<const Framebuffer*, 2> _framebufferBindings;
			const ShaderProgram* _shaderBinding;

			scp::f32vec4 _clearColor;
			double _clearDepth;
			int32_t _clearStencil;
			scp::i32vec2 _viewportOffset;
			scp::u32vec2 _viewportSize;
			bool _isDepthTestEnabled;

			std::queue<DebugMessage*> _debugMessages;
			DebugMessage* _lastDebugMessageSent;

		friend class ContextManager;
		friend class Window;
		friend void stackDebugMessage(DebugMessage* message, Context* context);
		friend class Buffer;
		friend class RawTexture;
		friend class Framebuffer;
		friend class ShaderProgram;
	};

	class ContextManager
	{
		public:

			ContextManager() = delete;

			static bool setCurrentContext(Context* context);

			static bool isInValidContext();
			static Context* getCurrentContext();

		private:

			static Context* _createContext();
			static bool _destroyContext(Context* context);


			static std::mutex s_mutex;
			static std::unordered_set<Context*> s_contexts;
			static std::unordered_map<std::thread::id, Context*> s_currentContexts;

		friend class Window;
	};
}
