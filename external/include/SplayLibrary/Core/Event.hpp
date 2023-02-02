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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Core
	/// \defgroup Events
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Event
	/// \enum EventType
	/// \brief Enum used for event specialization. Represents each type of event.
	/// 
	/// For more information about each type of event, see the corresponding structs.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class EventType
	{
		Unknown,				///< Used for invalid events and error checking.
		KeyboardEvent,			///< General purpose keyboard related events.
		TextEvent,				///< Keyboard event specialized in retrieving the text generated.
		MouseMoveEvent,			///< Event generated when the mouse is moved.
		MouseEnterWindowEvent,	///< Event generated when the mouse enters the window.
		MouseLeaveWindowEvent,	///< Event generated when the mouse leaves the window.
		MouseButtonEvent,		///< General purpose mouse button related events.
		ScrollEvent,			///< Event generated when the user "scrolls" (using the wheel of the mouse for instance).
		ResizeEvent				///< Event generated when the user resizes the window.
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct Event
	/// \brief Struct representing a window event.
	/// 
	/// To learn more about how event are generated, see class Window.
	/// 
	/// Events, in SplayLibrary, are simply structs inheriting from `Event`. When polling an event, you retrieve the
	/// memory address of the event. The only thing you know about this event is its type: `Event::type`.
	/// With a switch statement on it, you can then apply the good cast on the event to retrieve the corresponding
	/// structure.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the event casted to the struct corresponding to the `EventType` given in template parameter.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<EventType T> const typename EventTypeEncapsulator<T>::Type& specialize()
		{
			assert(T == type);
			return *static_cast<typename EventTypeEncapsulator<T>::Type*>(this);
		}

		EventType type = EventType::Unknown;	///< Type of event.
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates KeyboardEvent
	/// \enum KeyboardKey
	/// \brief Enum used to represent each key of the keyboard.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates KeyboardEvent MouseButtonEvent
	/// \enum ButtonAction
	/// \brief Enum used to represent each possible action done with a button.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class ButtonAction
	{
		Unknown,
		Pressed,
		Repeated,
		Released
	};

	namespace KeyboardModifier
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \relates KeyboardEvent MouseButtonEvent
		/// \enum ModifierFlags
		/// \brief Enum used to represent each possible modifier applicable to an action.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct KeyboardEvent
	/// \brief Struct representing a general keyboard event.
	/// 
	/// *WARNING* : The key here corresponds to the key of the standard US keyboard layout. If your intent is to
	/// retrieve a text input, do not use this event ! Instead, use `TextEvent`.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct KeyboardEvent : public Event
	{
		int scancode;								///< Corresponding scancode (hardware specific).
		KeyboardKey key;							///< Key involved (on the standard US keyboard layout).
		ButtonAction action;						///< Action associated.
		KeyboardModifier::ModifierFlags modifiers;	///< Contextual modifiers.
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct TextEvent
	/// \brief Struct representing text input event.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct TextEvent : public Event
	{
		char32_t unicode;	///< UTF-32 character entered.
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct MouseMoveEvent
	/// \brief Event sent when the use moves the mouse.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MouseMoveEvent : public Event
	{
		scp::f64vec2 pos;	///< New position of the mouse.
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct MouseEnterWindowEvent
	/// \brief Event sent when the mouse enters the window.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MouseEnterWindowEvent : public Event {};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct MouseLeaveWindowEvent
	/// \brief Event sent when the mouse leaves the window.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MouseLeaveWindowEvent : public Event {};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates MouseButtonEvent
	/// \enum MouseButton
	/// \brief Enum used to represent each button of a mouse.
	/// 
	/// For mouses that have more than 11 buttons: ¯\_(o_o)_/¯
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct MouseButtonEvent
	/// \brief Struct representing a general mouse button related event.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct MouseButtonEvent : public Event
	{
		MouseButton button;							///< Button involved.
		ButtonAction action;						///< Action associated.
		KeyboardModifier::ModifierFlags modifiers;	///< Contextual modifiers.
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct ScrollEvent
	/// \brief Event sent when the user scrolls.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ScrollEvent : public Event
	{
		scp::f64vec2 offset;	///< Scroll delta.
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ingroup Events
	/// \struct ResizeEvent
	/// \brief Event sent when the user resizes the window.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ResizeEvent : public Event
	{
		scp::u32vec2 size;	///< New window size.
	};
}
