///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2021-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <SplayLibrary/Core/CoreDecl.hpp>
#include <SplayLibrary/Private/PrivateIncluded.hpp>

namespace spl
{
	template<CGlslScalarType TScalar>
	void ShaderProgram::setUniform(const std::string& name, const TScalar& scalar) const
	{
		static constexpr GlslType type = _spl::glslScalarTypeToGlslType<TScalar>();

		if constexpr (type == GlslType::Bool)
		{
			const int32_t buffer = scalar;
			_setUniform(name, GlslType::Int, &buffer, 1);
		}
		else
		{
			_setUniform(name, type, &scalar, 1);
		}
	}

	template<CGlslScalarType TScalar>
	void ShaderProgram::setUniform(const std::string& name, const TScalar* scalars, uint32_t count) const
	{
		static constexpr GlslType type = _spl::glslScalarTypeToGlslType<TScalar>();

		if constexpr (type == GlslType::Bool)
		{
			std::vector<int32_t> buffer(count);
			std::copy_n(scalars, count, buffer.begin());

			_setUniform(name, GlslType::Int, buffer.data(), count);
		}
		else
		{
			_setUniform(name, type, scalars, count);
		}
	}

	template<CGlslVecType TVec>
	void ShaderProgram::setUniform(const std::string& name, const TVec& vec) const
	{
		static constexpr GlslType type = _spl::glslVecTypeToGlslType<TVec>();

		if constexpr (type == GlslType::BoolVec2)
		{
			const int32_t buffer[2] = { vec.x, vec.y };
			_setUniform(name, GlslType::IntVec2, buffer, 1);
		}
		else if constexpr (type == GlslType::BoolVec3)
		{
			const int32_t buffer[3] = { vec.x, vec.y, vec.z };
			_setUniform(name, GlslType::IntVec3, buffer, 1);
		}
		else if constexpr (type == GlslType::BoolVec4)
		{
			const int32_t buffer[4] = { vec.x, vec.y, vec.z, vec.w };
			_setUniform(name, GlslType::IntVec4, buffer, 1);
		}
		else
		{
			_setUniform(name, type, &vec, 1);
		}
	}

	template<CGlslVecType TVec>
	void ShaderProgram::setUniform(const std::string& name, const TVec* vecs, uint32_t count) const
	{
		static constexpr GlslType type = _spl::glslVecTypeToGlslType<TVec>();

		if constexpr (type == GlslType::BoolVec2)
		{
			std::vector<int32_t> buffer(2 * count);
			std::copy_n(reinterpret_cast<const bool*>(vecs), 2 * count, buffer.begin());

			_setUniform(name, GlslType::IntVec2, buffer.data(), count);
		}
		else if constexpr (type == GlslType::BoolVec3)
		{
			std::vector<int32_t> buffer(3 * count);
			std::copy_n(reinterpret_cast<const bool*>(vecs), 3 * count, buffer.begin());

			_setUniform(name, GlslType::IntVec3, buffer.data(), count);
		}
		else if constexpr (type == GlslType::BoolVec4)
		{
			std::vector<int32_t> buffer(4 * count);
			std::copy_n(reinterpret_cast<const bool*>(vecs), 4 * count, buffer.begin());

			_setUniform(name, GlslType::IntVec4, buffer.data(), count);
		}
		else
		{
			_setUniform(name, type, vecs, count);
		}
	}

	template<CGlslMatType TMat>
	void ShaderProgram::setUniform(const std::string& name, const TMat& mat) const
	{
		_setUniform(name, _spl::glslMatTypeToGlslType<TMat>(), &mat, 1);
	}

	template<CGlslMatType TMat>
	void ShaderProgram::setUniform(const std::string& name, const TMat* mats, uint32_t count) const
	{
		_setUniform(name, _spl::glslMatTypeToGlslType<TMat>(), mats, count);
	}
}
