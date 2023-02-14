///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreTypes.hpp>

namespace lys
{
	enum class MaterialType
	{
		Undefined,
		// FlatColor,
		BlinnPhong,
		// Fresnel
	};

	class MaterialBase
	{
		protected:

			MaterialBase() = default;

			MaterialBase(const MaterialBase& drawable) = default;
			MaterialBase(MaterialBase&& drawable) = default;

			MaterialBase& operator=(const MaterialBase& drawable) = default;
			MaterialBase& operator=(MaterialBase&& drawable) = default;

			virtual ~MaterialBase() = default;

		private:

			virtual MaterialType _getType() const = 0;

			virtual void _applyToShader(const spl::ShaderProgram* shader) const = 0;

		friend class Scene;
	};
}
