///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <atomic>
#include <bitset>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <SciPP/SciPPTypes.hpp>
#include <Diskon/DiskonTypes.hpp>
#include <DejaVu/DejaVuTypes.hpp>


namespace spl
{
	template<typename T> concept CGlslScalarType =
		std::same_as<T, float>
		|| std::same_as<T, double>
		|| std::same_as<T, bool>
		|| std::same_as<T, int32_t>
		|| std::same_as<T, uint32_t>;

	template<typename T> concept CGlslVecType =
		std::same_as<T, scp::f32vec2> || std::same_as<T, scp::f32vec3> || std::same_as<T, scp::f32vec4>
		|| std::same_as<T, scp::f64vec2> || std::same_as<T, scp::f64vec3> || std::same_as<T, scp::f64vec4>
		|| std::same_as<T, scp::bvec2> || std::same_as<T, scp::bvec3> || std::same_as<T, scp::bvec4>
		|| std::same_as<T, scp::i32vec2> || std::same_as<T, scp::i32vec3> || std::same_as<T, scp::i32vec4>
		|| std::same_as<T, scp::u32vec2> || std::same_as<T, scp::u32vec3> || std::same_as<T, scp::u32vec4>;

	template<typename T> concept CGlslMatType =
		std::same_as<T, scp::f32mat2x2> || std::same_as<T, scp::f32mat2x3> || std::same_as<T, scp::f32mat2x4>
		|| std::same_as<T, scp::f32mat3x2> || std::same_as<T, scp::f32mat3x3> || std::same_as<T, scp::f32mat3x4>
		|| std::same_as<T, scp::f32mat4x2> || std::same_as<T, scp::f32mat4x3> || std::same_as<T, scp::f32mat4x4>
		|| std::same_as<T, scp::f64mat2x2> || std::same_as<T, scp::f64mat2x3> || std::same_as<T, scp::f64mat2x4>
		|| std::same_as<T, scp::f64mat3x2> || std::same_as<T, scp::f64mat3x3> || std::same_as<T, scp::f64mat3x4>
		|| std::same_as<T, scp::f64mat4x2> || std::same_as<T, scp::f64mat4x3> || std::same_as<T, scp::f64mat4x4>;

	// TODO: Rename float16_t in half_t, remove float32_t and float64_t and add half_t here...
	// TODO: Find a way to create scp::f16vecx and add them here... (table 8.16 of opengl 4.6 spec)
	template<typename T> concept CTextureInternalFormatType =
		std::same_as<T, float> || std::same_as<T, int8_t> || std::same_as<T, int16_t> || std::same_as<T, int32_t> || std::same_as<T, uint8_t> || std::same_as<T, uint16_t> || std::same_as<T, uint32_t>
		|| std::same_as<T, scp::f32vec2> || std::same_as<T, scp::i8vec2> || std::same_as<T, scp::i16vec2> || std::same_as<T, scp::i32vec2> || std::same_as<T, scp::u8vec2> || std::same_as<T, scp::u16vec2> || std::same_as<T, scp::u32vec2>
		|| std::same_as<T, scp::f32vec3> || std::same_as<T, scp::i32vec3> || std::same_as<T, scp::u32vec3>
		|| std::same_as<T, scp::f32vec4> || std::same_as<T, scp::i8vec4> || std::same_as<T, scp::i16vec4> || std::same_as<T, scp::i32vec4> || std::same_as<T, scp::u8vec4> || std::same_as<T, scp::u16vec4> || std::same_as<T, scp::u32vec4>;


	enum class BufferTarget;
	enum class BufferUsage;
	namespace BufferStorageFlags { enum Flags; }
	namespace BufferMapAccessFlags { enum Flags; }
	class Buffer;


	enum class TextureTarget;
	enum class TextureFormat;
	enum class TextureDataType;
	enum class TextureInternalFormat : uint64_t;
	class RawTexture;
	class Renderbuffer;

	class TextureBase;
	class Texture2D;


	namespace ShaderStage { enum Stage; }
	class ShaderModule;

	namespace ShaderProgramFlags { enum Flags; }
	enum class ShaderProgramInterface;
	enum class GlslType;
	struct ShaderProgramInterfaceInfos;
	struct ShaderProgramResourceInfos;
	class ShaderProgram;


	enum class PrimitiveType;
	enum class IndexType;
	class VertexArray;


	enum class FramebufferTarget;
	enum class FramebufferAttachment;
	class Framebuffer;
	class DefaultFramebuffer;


	enum class EventType;
	struct Event;
	enum class KeyboardKey;
	enum class ButtonAction;
	namespace KeyboardModifier { enum ModifierFlags; };
	struct KeyboardEvent;
	struct TextEvent;
	struct MouseMoveEvent;
	struct MouseEnterWindowEvent;
	struct MouseLeaveWindowEvent;
	struct MouseButtonEvent;
	struct ScrollEvent;
	struct ResizeEvent;
	
	enum class CursorMode;
	class Window;

	enum class DebugMessageSource;
	enum class DebugMessageType;
	enum class DebugMessageSeverity;
	struct DebugMessage;
	class Context;
	class ContextManager;
}
