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
	enum class EventType
	{
		Unknown,
		KeyboardEvent,
		TextEvent,
		MouseMoveEvent,
		MouseEnterWindowEvent,
		MouseLeaveWindowEvent,
		MouseButtonEvent,
		ScrollEvent,
		ResizeEvent
	};


	struct Event
	{
		template<EventType T> struct EventTypeEncapsulator { using Type = Event; };
		template<> struct EventTypeEncapsulator<EventType::KeyboardEvent> { using Type = KeyboardEvent; };
		template<> struct EventTypeEncapsulator<EventType::TextEvent> { using Type = TextEvent; };
		template<> struct EventTypeEncapsulator<EventType::MouseMoveEvent> { using Type = MouseMoveEvent; };
		template<> struct EventTypeEncapsulator<EventType::MouseEnterWindowEvent> { using Type = MouseEnterWindowEvent; };
		template<> struct EventTypeEncapsulator<EventType::MouseLeaveWindowEvent> { using Type = MouseLeaveWindowEvent; };
		template<> struct EventTypeEncapsulator<EventType::MouseButtonEvent> { using Type = MouseButtonEvent; };
		template<> struct EventTypeEncapsulator<EventType::ScrollEvent> { using Type = ScrollEvent; };
		template<> struct EventTypeEncapsulator<EventType::ResizeEvent> { using Type = ResizeEvent; };

		template<EventType T> const typename EventTypeEncapsulator<T>::Type& specialize()
		{
			assert(T == type);
			return *static_cast<typename EventTypeEncapsulator<T>::Type*>(this);
		}

		EventType type = EventType::Unknown;	///< Type of event.
	};


	enum class KeyboardKey
	{
		Unknown,
		Space,
		Apostrophe,
		Comma,
		Minus,
		Period,
		Slash,
		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		Semicolon,
		Equal,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LeftBracket,
		Backslash,
		RightBracket,
		GraveAccent,
		World1,
		World2,
		Escape,
		Enter,
		Tab,
		Backspace,
		Insert,
		Delete,
		Right,
		Left,
		Down,
		Up,
		PageUp,
		PageDown,
		Home,
		End,
		CapsLock,
		ScrollLock,
		NumLock,
		PrintScreen,
		Pause,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		NumpadDecimal,
		NumpadDivide,
		NumpadMultiply,
		NumpadSubtract,
		NumpadAdd,
		NumpadEnter,
		NumpadEqual,
		LeftShift,
		LeftControl,
		LeftAlt,
		LeftSuper,
		RightShift,
		RightControl,
		RightAlt,
		RightSuper,
		Menu
	};


	enum class ButtonAction
	{
		Unknown,
		Pressed,
		Repeated,
		Released
	};


	namespace KeyboardModifier
	{
		enum ModifierFlags
		{
			None		= 0,
			Shift		= 1 << 0,
			Control		= 1 << 1,
			Alt			= 1 << 2,
			Super		= 1 << 3,
			CapsLock	= 1 << 4,
			NumLock		= 1 << 5
		};
	}


	struct KeyboardEvent : public Event
	{
		int scancode;
		KeyboardKey key;
		ButtonAction action;
		KeyboardModifier::ModifierFlags modifiers;
	};


	struct TextEvent : public Event
	{
		char32_t unicode;
	};


	struct MouseMoveEvent : public Event
	{
		scp::f64vec2 pos;
	};


	struct MouseEnterWindowEvent : public Event {};


	struct MouseLeaveWindowEvent : public Event {};


	enum class MouseButton
	{
		Unknown,
		Left,
		Right,
		Middle,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8
	};


	struct MouseButtonEvent : public Event
	{
		MouseButton button;
		ButtonAction action;
		KeyboardModifier::ModifierFlags modifiers;
	};


	struct ScrollEvent : public Event
	{
		scp::f64vec2 offset;
	};


	struct ResizeEvent : public Event
	{
		scp::u32vec2 size;
	};
}
