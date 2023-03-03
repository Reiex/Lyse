///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Private/PrivateIncluded.hpp>

namespace spl
{
	namespace _spl
	{
		constexpr bool isIndexedBufferTarget(BufferTarget target)
		{
			switch (target)
			{
				case BufferTarget::Array:
				case BufferTarget::CopyRead:
				case BufferTarget::CopyWrite:
				case BufferTarget::DispatchIndirect:
				case BufferTarget::DrawIndirect:
				case BufferTarget::ElementArray:
				case BufferTarget::Parameter:
				case BufferTarget::PixelPack:
				case BufferTarget::PixelUnpack:
				case BufferTarget::Query:
				case BufferTarget::Texture:
					return false;
				case BufferTarget::AtomicCounter:
				case BufferTarget::ShaderStorage:
				case BufferTarget::TransformFeedback:
				case BufferTarget::Uniform:
					return true;
				default:
					assert(false);
					return false;
			}
		}


		template<CGlslScalarType TScalar>
		consteval GlslType glslScalarTypeToGlslType()
		{
			if constexpr (std::same_as<TScalar, float>) { return GlslType::Float; }
			else if constexpr (std::same_as<TScalar, double>) { return GlslType::Double; }
			else if constexpr (std::same_as<TScalar, bool>) { return GlslType::Bool; }
			else if constexpr (std::same_as<TScalar, int32_t>) { return GlslType::Int; }
			else if constexpr (std::same_as<TScalar, uint32_t>) { return GlslType::UnsignedInt; }
			else { assert(false); return GlslType::Undefined; }
		}

		template<CGlslVecType TVec>
		consteval GlslType glslVecTypeToGlslType()
		{
			if constexpr (std::same_as<TVec, scp::f32vec2>) { return GlslType::FloatVec2; }
			else if constexpr (std::same_as<TVec, scp::f32vec3>) { return GlslType::FloatVec3; }
			else if constexpr (std::same_as<TVec, scp::f32vec4>) { return GlslType::FloatVec4; }
			else if constexpr (std::same_as<TVec, scp::f64vec2>) { return GlslType::DoubleVec2; }
			else if constexpr (std::same_as<TVec, scp::f64vec3>) { return GlslType::DoubleVec3; }
			else if constexpr (std::same_as<TVec, scp::f64vec4>) { return GlslType::DoubleVec4; }
			else if constexpr (std::same_as<TVec, scp::bvec2>) { return GlslType::BoolVec2; }
			else if constexpr (std::same_as<TVec, scp::bvec3>) { return GlslType::BoolVec3; }
			else if constexpr (std::same_as<TVec, scp::bvec4>) { return GlslType::BoolVec4; }
			else if constexpr (std::same_as<TVec, scp::i32vec2>) { return GlslType::IntVec2; }
			else if constexpr (std::same_as<TVec, scp::i32vec3>) { return GlslType::IntVec3; }
			else if constexpr (std::same_as<TVec, scp::i32vec4>) { return GlslType::IntVec4; }
			else if constexpr (std::same_as<TVec, scp::u32vec2>) { return GlslType::UnsignedIntVec2; }
			else if constexpr (std::same_as<TVec, scp::u32vec3>) { return GlslType::UnsignedIntVec3; }
			else if constexpr (std::same_as<TVec, scp::u32vec4>) { return GlslType::UnsignedIntVec4; }
			else { assert(false); return GlslType::Undefined; }
		}

		template<CGlslMatType TMat>
		consteval GlslType glslMatTypeToGlslType()
		{
			if constexpr (std::same_as<TMat, scp::f32mat2x2>) { return GlslType::FloatMat2x2; }
			else if constexpr (std::same_as<TMat, scp::f32mat2x3>) { return GlslType::FloatMat2x3; }
			else if constexpr (std::same_as<TMat, scp::f32mat2x4>) { return GlslType::FloatMat2x4; }
			else if constexpr (std::same_as<TMat, scp::f32mat3x2>) { return GlslType::FloatMat3x2; }
			else if constexpr (std::same_as<TMat, scp::f32mat3x3>) { return GlslType::FloatMat3x3; }
			else if constexpr (std::same_as<TMat, scp::f32mat3x4>) { return GlslType::FloatMat3x4; }
			else if constexpr (std::same_as<TMat, scp::f32mat4x2>) { return GlslType::FloatMat4x2; }
			else if constexpr (std::same_as<TMat, scp::f32mat4x3>) { return GlslType::FloatMat4x3; }
			else if constexpr (std::same_as<TMat, scp::f32mat4x4>) { return GlslType::FloatMat4x4; }
			else if constexpr (std::same_as<TMat, scp::f64mat2x2>) { return GlslType::DoubleMat2x2; }
			else if constexpr (std::same_as<TMat, scp::f64mat2x3>) { return GlslType::DoubleMat2x3; }
			else if constexpr (std::same_as<TMat, scp::f64mat2x4>) { return GlslType::DoubleMat2x4; }
			else if constexpr (std::same_as<TMat, scp::f64mat3x2>) { return GlslType::DoubleMat3x2; }
			else if constexpr (std::same_as<TMat, scp::f64mat3x3>) { return GlslType::DoubleMat3x3; }
			else if constexpr (std::same_as<TMat, scp::f64mat3x4>) { return GlslType::DoubleMat3x4; }
			else if constexpr (std::same_as<TMat, scp::f64mat4x2>) { return GlslType::DoubleMat4x2; }
			else if constexpr (std::same_as<TMat, scp::f64mat4x3>) { return GlslType::DoubleMat4x3; }
			else if constexpr (std::same_as<TMat, scp::f64mat4x4>) { return GlslType::DoubleMat4x4; }
			else { assert(false); return GlslType::Undefined; }
		}


		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_u8>					{ using Type = uint8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_i8>					{ using Type = int8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_nu8>					{ using Type = uint8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_ni8>					{ using Type = int8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_u16>					{ using Type = uint16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_i16>					{ using Type = int16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_f16>					{ using Type = float; };	// TODO
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_nu16>				{ using Type = uint16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_ni16>				{ using Type = int16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_u32>					{ using Type = uint32_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_i32>					{ using Type = int32_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_f32>					{ using Type = float; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_u8>					{ using Type = scp::u8vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_i8>					{ using Type = scp::i8vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_nu8>				{ using Type = scp::u8vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_ni8>				{ using Type = scp::i8vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_u16>				{ using Type = scp::u16vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_i16>				{ using Type = scp::i16vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_f16>				{ using Type = scp::f32vec2; };	// TODO
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_nu16>				{ using Type = scp::u16vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_ni16>				{ using Type = scp::i16vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_u32>				{ using Type = scp::u32vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_i32>				{ using Type = scp::i32vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RG_f32>				{ using Type = scp::f32vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_u8>				{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_i8>				{ using Type = scp::i8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu8>				{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_ni8>				{ using Type = scp::i8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_u16>				{ using Type = scp::u16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_i16>				{ using Type = scp::i16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_f16>				{ using Type = scp::f32vec3; };	// TODO
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu16>				{ using Type = scp::u16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_ni16>				{ using Type = scp::i16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_u32>				{ using Type = scp::u32vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_i32>				{ using Type = scp::i32vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_f32>				{ using Type = scp::f32vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_u8>				{ using Type = scp::u8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_i8>				{ using Type = scp::i8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_nu8>				{ using Type = scp::u8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_ni8>				{ using Type = scp::i8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_u16>				{ using Type = scp::u16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_i16>				{ using Type = scp::i16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_f16>				{ using Type = scp::f32vec4; };	// TODO
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_nu16>				{ using Type = scp::u16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_ni16>				{ using Type = scp::i16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_u32>				{ using Type = scp::u32vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_i32>				{ using Type = scp::i32vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_f32>				{ using Type = scp::f32vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_nu16>			{ using Type = uint16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_nu24>			{ using Type = uint32_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_nu32>			{ using Type = uint32_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_f32>				{ using Type = float; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Stencil_u1>			{ using Type = uint8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Stencil_u4>			{ using Type = uint8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Stencil_u8>			{ using Type = uint8_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Stencil_u16>			{ using Type = uint16_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_nu24_Stencil_u8>	{ using Type = uint32_t; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::Depth_f32_Stencil_u8>	{ using Type = scp::f32vec2; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu4>				{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu5>				{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu10>				{ using Type = scp::u16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu12>				{ using Type = scp::u16vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_nu3_G_nu3_B_nu2>		{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_nu5_G_nu6_B_nu5>		{ using Type = scp::u8vec3; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::R_f11_G_f11_B_f10>		{ using Type = scp::f32vec3; };	// TODO
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_nu2>				{ using Type = scp::u8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_nu4>				{ using Type = scp::u8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGBA_nu12>				{ using Type = scp::u16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu5_A_nu1>			{ using Type = scp::u8vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_u10_A_u2>			{ using Type = scp::u16vec4; };
		template<> struct TextureInternalFormatToColorVecType<TextureInternalFormat::RGB_nu10_A_nu2>		{ using Type = scp::u16vec4; };
		
		template<> struct ColorVecTypeToPixelType<int8_t>		{ using Type = djv::Pixel_gs_i8; };
		template<> struct ColorVecTypeToPixelType<uint8_t>		{ using Type = djv::Pixel_gs_u8; };
		template<> struct ColorVecTypeToPixelType<int16_t>		{ using Type = djv::Pixel_gs_i16; };
		template<> struct ColorVecTypeToPixelType<uint16_t>		{ using Type = djv::Pixel_gs_u16; };
		template<> struct ColorVecTypeToPixelType<int32_t>		{ using Type = djv::Pixel_gs_i32; };
		template<> struct ColorVecTypeToPixelType<uint32_t>		{ using Type = djv::Pixel_gs_u32; };
		template<> struct ColorVecTypeToPixelType<float>		{ using Type = djv::Pixel_gs_f32; };
		template<> struct ColorVecTypeToPixelType<scp::i8vec2>	{ using Type = djv::Pixel_rg_i8; };
		template<> struct ColorVecTypeToPixelType<scp::u8vec2>	{ using Type = djv::Pixel_rg_u8; };
		template<> struct ColorVecTypeToPixelType<scp::i16vec2>	{ using Type = djv::Pixel_rg_i16; };
		template<> struct ColorVecTypeToPixelType<scp::u16vec2>	{ using Type = djv::Pixel_rg_u16; };
		template<> struct ColorVecTypeToPixelType<scp::i32vec2>	{ using Type = djv::Pixel_rg_i32; };
		template<> struct ColorVecTypeToPixelType<scp::u32vec2>	{ using Type = djv::Pixel_rg_u32; };
		template<> struct ColorVecTypeToPixelType<scp::f32vec2>	{ using Type = djv::Pixel_rg_f32; };
		template<> struct ColorVecTypeToPixelType<scp::i8vec3>	{ using Type = djv::Pixel_rgb_i8; };
		template<> struct ColorVecTypeToPixelType<scp::u8vec3>	{ using Type = djv::Pixel_rgb_u8; };
		template<> struct ColorVecTypeToPixelType<scp::i16vec3>	{ using Type = djv::Pixel_rgb_i16; };
		template<> struct ColorVecTypeToPixelType<scp::u16vec3>	{ using Type = djv::Pixel_rgb_u16; };
		template<> struct ColorVecTypeToPixelType<scp::i32vec3>	{ using Type = djv::Pixel_rgb_i32; };
		template<> struct ColorVecTypeToPixelType<scp::u32vec3>	{ using Type = djv::Pixel_rgb_u32; };
		template<> struct ColorVecTypeToPixelType<scp::f32vec3>	{ using Type = djv::Pixel_rgb_f32; };
		template<> struct ColorVecTypeToPixelType<scp::i8vec4>	{ using Type = djv::Pixel_rgba_i8; };
		template<> struct ColorVecTypeToPixelType<scp::u8vec4>	{ using Type = djv::Pixel_rgba_u8; };
		template<> struct ColorVecTypeToPixelType<scp::i16vec4>	{ using Type = djv::Pixel_rgba_i16; };
		template<> struct ColorVecTypeToPixelType<scp::u16vec4>	{ using Type = djv::Pixel_rgba_u16; };
		template<> struct ColorVecTypeToPixelType<scp::i32vec4>	{ using Type = djv::Pixel_rgba_i32; };
		template<> struct ColorVecTypeToPixelType<scp::u32vec4>	{ using Type = djv::Pixel_rgba_u32; };
		template<> struct ColorVecTypeToPixelType<scp::f32vec4> { using Type = djv::Pixel_rgba_f32; };
		
		template<CColorVecType TColorVec>
		consteval TextureInternalFormat colorVecTypeToTextureInternalFormat()
		{
			if constexpr (std::same_as<TColorVec, int8_t>)				{ return TextureInternalFormat::R_ni8; }
			else if constexpr (std::same_as<TColorVec, uint8_t>)		{ return TextureInternalFormat::R_nu8; }
			else if constexpr (std::same_as<TColorVec, int16_t>)		{ return TextureInternalFormat::R_ni16; }
			else if constexpr (std::same_as<TColorVec, uint16_t>)		{ return TextureInternalFormat::R_nu16; }
			else if constexpr (std::same_as<TColorVec, int32_t>)		{ return TextureInternalFormat::R_i32; }
			else if constexpr (std::same_as<TColorVec, uint32_t>)		{ return TextureInternalFormat::R_u32; }
			else if constexpr (std::same_as<TColorVec, float>)			{ return TextureInternalFormat::R_f32; }
			else if constexpr (std::same_as<TColorVec, scp::i8vec2>)	{ return TextureInternalFormat::RG_ni8; }
			else if constexpr (std::same_as<TColorVec, scp::u8vec2>)	{ return TextureInternalFormat::RG_nu8; }
			else if constexpr (std::same_as<TColorVec, scp::i16vec2>)	{ return TextureInternalFormat::RG_ni16; }
			else if constexpr (std::same_as<TColorVec, scp::u16vec2>)	{ return TextureInternalFormat::RG_nu16; }
			else if constexpr (std::same_as<TColorVec, scp::i32vec2>)	{ return TextureInternalFormat::RG_i32; }
			else if constexpr (std::same_as<TColorVec, scp::u32vec2>)	{ return TextureInternalFormat::RG_u32; }
			else if constexpr (std::same_as<TColorVec, scp::f32vec2>)	{ return TextureInternalFormat::RG_f32; }
			else if constexpr (std::same_as<TColorVec, scp::i8vec3>)	{ return TextureInternalFormat::RGB_ni8; }
			else if constexpr (std::same_as<TColorVec, scp::u8vec3>)	{ return TextureInternalFormat::RGB_nu8; }
			else if constexpr (std::same_as<TColorVec, scp::i16vec3>)	{ return TextureInternalFormat::RGB_ni16; }
			else if constexpr (std::same_as<TColorVec, scp::u16vec3>)	{ return TextureInternalFormat::RGB_nu16; }
			else if constexpr (std::same_as<TColorVec, scp::i32vec3>)	{ return TextureInternalFormat::RGB_i32; }
			else if constexpr (std::same_as<TColorVec, scp::u32vec3>)	{ return TextureInternalFormat::RGB_u32; }
			else if constexpr (std::same_as<TColorVec, scp::f32vec3>)	{ return TextureInternalFormat::RGB_f32; }
			else if constexpr (std::same_as<TColorVec, scp::i8vec4>)	{ return TextureInternalFormat::RGBA_ni8; }
			else if constexpr (std::same_as<TColorVec, scp::u8vec4>)	{ return TextureInternalFormat::RGBA_nu8; }
			else if constexpr (std::same_as<TColorVec, scp::i16vec4>)	{ return TextureInternalFormat::RGBA_ni16; }
			else if constexpr (std::same_as<TColorVec, scp::u16vec4>)	{ return TextureInternalFormat::RGBA_nu16; }
			else if constexpr (std::same_as<TColorVec, scp::i32vec4>)	{ return TextureInternalFormat::RGBA_i32; }
			else if constexpr (std::same_as<TColorVec, scp::u32vec4>)	{ return TextureInternalFormat::RGBA_u32; }
			else if constexpr (std::same_as<TColorVec, scp::f32vec4>)	{ return TextureInternalFormat::RGBA_f32; }
			else { assert(false); }
		}
		
		constexpr TextureFormat textureInternalFormatToTextureFormat(TextureInternalFormat internalFormat)
		{
			switch (internalFormat)
			{
				case TextureInternalFormat::R_u8:
				case TextureInternalFormat::R_i8:
				case TextureInternalFormat::R_u16:
				case TextureInternalFormat::R_i16:
				case TextureInternalFormat::R_u32:
				case TextureInternalFormat::R_i32:
					return TextureFormat::iR;
		
				case TextureInternalFormat::R_nu8:
				case TextureInternalFormat::R_ni8:
				case TextureInternalFormat::R_f16:
				case TextureInternalFormat::R_nu16:
				case TextureInternalFormat::R_ni16:
				case TextureInternalFormat::R_f32:
					return TextureFormat::R;
		
				case TextureInternalFormat::RG_u8:
				case TextureInternalFormat::RG_i8:
				case TextureInternalFormat::RG_u16:
				case TextureInternalFormat::RG_i16:
				case TextureInternalFormat::RG_u32:
				case TextureInternalFormat::RG_i32:
					return TextureFormat::iRG;
		
				case TextureInternalFormat::RG_nu8:
				case TextureInternalFormat::RG_ni8:
				case TextureInternalFormat::RG_f16:
				case TextureInternalFormat::RG_nu16:
				case TextureInternalFormat::RG_ni16:
				case TextureInternalFormat::RG_f32:
					return TextureFormat::RG;
		
				case TextureInternalFormat::RGB_u8:
				case TextureInternalFormat::RGB_i8:
				case TextureInternalFormat::RGB_u16:
				case TextureInternalFormat::RGB_i16:
				case TextureInternalFormat::RGB_u32:
				case TextureInternalFormat::RGB_i32:
					return TextureFormat::iRGB;
		
				case TextureInternalFormat::RGB_nu8:
				case TextureInternalFormat::RGB_ni8:
				case TextureInternalFormat::RGB_f16:
				case TextureInternalFormat::RGB_nu16:
				case TextureInternalFormat::RGB_ni16:
				case TextureInternalFormat::RGB_f32:
				case TextureInternalFormat::R_f11_G_f11_B_f10:
				case TextureInternalFormat::RGB_nu4:
				case TextureInternalFormat::RGB_nu5:
				case TextureInternalFormat::RGB_nu10:
				case TextureInternalFormat::RGB_nu12:
				case TextureInternalFormat::R_nu3_G_nu3_B_nu2:
				case TextureInternalFormat::R_nu5_G_nu6_B_nu5:
					return TextureFormat::RGB;
		
				case TextureInternalFormat::RGBA_u8:
				case TextureInternalFormat::RGBA_i8:
				case TextureInternalFormat::RGBA_u16:
				case TextureInternalFormat::RGBA_i16:
				case TextureInternalFormat::RGBA_u32:
				case TextureInternalFormat::RGBA_i32:
				case TextureInternalFormat::RGB_u10_A_u2:
					return TextureFormat::iRGBA;
		
				case TextureInternalFormat::RGBA_nu8:
				case TextureInternalFormat::RGBA_ni8:
				case TextureInternalFormat::RGBA_f16:
				case TextureInternalFormat::RGBA_nu16:
				case TextureInternalFormat::RGBA_ni16:
				case TextureInternalFormat::RGBA_f32:
				case TextureInternalFormat::RGBA_nu2:
				case TextureInternalFormat::RGBA_nu4:
				case TextureInternalFormat::RGBA_nu12:
				case TextureInternalFormat::RGB_nu5_A_nu1:
				case TextureInternalFormat::RGB_nu10_A_nu2:
					return TextureFormat::RGBA;
		
				case TextureInternalFormat::Depth_nu16:
				case TextureInternalFormat::Depth_nu24:
				case TextureInternalFormat::Depth_nu32:
				case TextureInternalFormat::Depth_f32:
					return TextureFormat::DepthComponent;
		
				case TextureInternalFormat::Stencil_u1:
				case TextureInternalFormat::Stencil_u4:
				case TextureInternalFormat::Stencil_u8:
				case TextureInternalFormat::Stencil_u16:
					return TextureFormat::StencilIndex;
		
				case TextureInternalFormat::Depth_nu24_Stencil_u8:
				case TextureInternalFormat::Depth_f32_Stencil_u8:
					return TextureFormat::DepthStencil;
		
				default:
					assert(false);
					return TextureFormat::Undefined;
			}
		}
		
		constexpr TextureDataType textureInternalFormatToTextureDataType(TextureInternalFormat internalFormat)
		{
			switch (internalFormat)
			{
				case TextureInternalFormat::R_u8:
				case TextureInternalFormat::R_nu8:
				case TextureInternalFormat::RG_u8:
				case TextureInternalFormat::RG_nu8:
				case TextureInternalFormat::RGB_u8:
				case TextureInternalFormat::RGB_nu8:
				case TextureInternalFormat::RGBA_u8:
				case TextureInternalFormat::RGBA_nu8:
				case TextureInternalFormat::Stencil_u1:
				case TextureInternalFormat::Stencil_u4:
				case TextureInternalFormat::Stencil_u8:
				case TextureInternalFormat::RGB_nu4:	// TODO: TextureDataType::UnsignedShort_4_4_4_4 ?
				case TextureInternalFormat::RGB_nu5:	// TODO: TextureDataType::UnsignedShort_5_5_5_1 ?
				case TextureInternalFormat::RGBA_nu2:
					return TextureDataType::UnsignedByte;
		
				case TextureInternalFormat::R_i8:
				case TextureInternalFormat::R_ni8:
				case TextureInternalFormat::RG_i8:
				case TextureInternalFormat::RG_ni8:
				case TextureInternalFormat::RGB_i8:
				case TextureInternalFormat::RGB_ni8:
				case TextureInternalFormat::RGBA_i8:
				case TextureInternalFormat::RGBA_ni8:
					return TextureDataType::Byte;
		
				case TextureInternalFormat::R_u16:
				case TextureInternalFormat::R_nu16:
				case TextureInternalFormat::RG_u16:
				case TextureInternalFormat::RG_nu16:
				case TextureInternalFormat::RGB_u16:
				case TextureInternalFormat::RGB_nu16:
				case TextureInternalFormat::RGBA_u16:
				case TextureInternalFormat::RGBA_nu16:
				case TextureInternalFormat::Depth_nu16:
				case TextureInternalFormat::Stencil_u16:
				case TextureInternalFormat::RGB_nu10:	// TODO: TextureDataType::UnsignedInt_10_10_10_2 ?
				case TextureInternalFormat::RGB_nu12:
				case TextureInternalFormat::RGBA_nu12:
					return TextureDataType::UnsignedShort;
		
				case TextureInternalFormat::R_i16:
				case TextureInternalFormat::R_ni16:
				case TextureInternalFormat::RG_i16:
				case TextureInternalFormat::RG_ni16:
				case TextureInternalFormat::RGB_i16:
				case TextureInternalFormat::RGB_ni16:
				case TextureInternalFormat::RGBA_i16:
				case TextureInternalFormat::RGBA_ni16:
					return TextureDataType::Short;
		
				case TextureInternalFormat::R_f16:
				case TextureInternalFormat::RG_f16:
				case TextureInternalFormat::RGB_f16:
				case TextureInternalFormat::RGBA_f16:
					return TextureDataType::HalfFloat;
		
				case TextureInternalFormat::R_u32:
				case TextureInternalFormat::RG_u32:
				case TextureInternalFormat::RGB_u32:
				case TextureInternalFormat::RGBA_u32:
				case TextureInternalFormat::Depth_nu24:
				case TextureInternalFormat::Depth_nu32:
					return TextureDataType::UnsignedInt;
		
				case TextureInternalFormat::R_i32:
				case TextureInternalFormat::RG_i32:
				case TextureInternalFormat::RGB_i32:
				case TextureInternalFormat::RGBA_i32:
					return TextureDataType::Int;
		
				case TextureInternalFormat::R_f32:
				case TextureInternalFormat::RG_f32:
				case TextureInternalFormat::RGB_f32:
				case TextureInternalFormat::RGBA_f32:
				case TextureInternalFormat::Depth_f32:
					return TextureDataType::Float;
		
				case TextureInternalFormat::Depth_nu24_Stencil_u8:
					return TextureDataType::UnsignedInt_24_8;
		
				case TextureInternalFormat::Depth_f32_Stencil_u8:
					return TextureDataType::Float32_UnsignedInt_24_8_Rev;
		
				case TextureInternalFormat::R_nu3_G_nu3_B_nu2:
					return TextureDataType::UnsignedByte_3_3_2;
		
				case TextureInternalFormat::R_nu5_G_nu6_B_nu5:
					return TextureDataType::UnsignedShort_5_6_5;
		
				case TextureInternalFormat::R_f11_G_f11_B_f10:
					return TextureDataType::UnsignedInt_10F_11F_11F_Rev;
		
				case TextureInternalFormat::RGBA_nu4:
					return TextureDataType::UnsignedShort_4_4_4_4;
		
				case TextureInternalFormat::RGB_nu5_A_nu1:
					return TextureDataType::UnsignedShort_5_5_5_1;
		
				case TextureInternalFormat::RGB_u10_A_u2:
				case TextureInternalFormat::RGB_nu10_A_nu2:
					return TextureDataType::UnsignedInt_10_10_10_2;
		
				default:
					assert(false);
					return TextureDataType::Undefined;
			}
		}
	}
}
