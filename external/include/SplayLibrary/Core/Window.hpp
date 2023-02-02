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
	enum class CursorMode
	{
		Normal,
		Hidden,
		Disabled
	};

	class Window
	{
		public:

			// TODO: Cursor object
			// TODO: Shared context (here and in ContextManager)

			Window(const scp::u32vec2& size, const std::string& title, bool debugContext);
			Window(const Window& window) = delete;
			Window(Window&& window) = delete;

			const Window& operator=(const Window& window) = delete;
			const Window& operator=(Window&& window) = delete;

			bool pollEvent(Event*& event);
			bool waitEvent(Event*& event, double timeout = 0.0);

			void display();

			void setCursorMode(CursorMode mode);

			bool shouldClose() const;
			bool isKeyPressed(KeyboardKey key) const;
			scp::f64vec2 getMousePosition() const;
			bool isCursorInWindow() const;
			bool isMouseButtonPressed(MouseButton button) const;

			bool isValid() const;
			void* getHandle();
			Context* getContext();
			const void* getHandle() const;
			const scp::u32vec2& getSize() const;
			DefaultFramebuffer& getFramebuffer();

			~Window();

		private:

			Window();

			bool _processEvent(Event*& event);

			void* _window;
			Context* _context;
			scp::u32vec2 _size;

			DefaultFramebuffer _framebuffer;

			std::queue<Event*> _events;
			Event* _lastEventSent;

		friend void stackEvent(void* window, Event* event);
	};
}
