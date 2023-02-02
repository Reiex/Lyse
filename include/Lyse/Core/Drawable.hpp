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
	class Drawable : public SceneDrawable
	{
		public:

			Drawable(const Drawable& drawable) = delete;
			Drawable(Drawable&& drawable) = delete;

			Drawable& operator=(const Drawable& drawable) = delete;
			Drawable& operator=(Drawable&& drawable) = delete;

			// This draw doesn't suppose anything about context, it's just a draw command
			virtual void draw() const = 0;

			virtual ~Drawable() override = default;

		protected:

			Drawable() = default;

		private:

			virtual void draw(const spl::ShaderProgram& program, const scp::f32mat4x4& transform) const override = 0;
	};
}
