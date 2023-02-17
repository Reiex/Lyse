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
	class DrawableGroup : public Drawable
	{
		public:

			virtual DrawableType getType() const override = 0;

		protected:

			DrawableGroup() = default;

			DrawableGroup(const DrawableGroup& group) = default;
			DrawableGroup(DrawableGroup&& group) = default;

			DrawableGroup& operator=(const DrawableGroup& group) = default;
			DrawableGroup& operator=(DrawableGroup&& group) = default;

			virtual ~DrawableGroup() override = default;

		private:

			virtual void _draw() const override = 0;
			virtual uint32_t _getChildCount() const = 0;
			virtual const Drawable* _getChild(uint32_t index) const = 0;

		friend class Scene;
	};
}
