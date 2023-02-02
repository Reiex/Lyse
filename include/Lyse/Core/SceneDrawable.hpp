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
	class SceneDrawable : public Transformable
	{
		public:

			SceneDrawable(const SceneDrawable& drawable) = delete;
			SceneDrawable(SceneDrawable&& drawable) = delete;

			SceneDrawable& operator=(const SceneDrawable& drawable) = delete;
			SceneDrawable& operator=(SceneDrawable&& drawable) = delete;

			// This draw is supposed to be called only in a scene hierarchy 
			virtual void draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const = 0;

			virtual ~SceneDrawable() override = default;

		protected:

			SceneDrawable() = default;
	};
}
