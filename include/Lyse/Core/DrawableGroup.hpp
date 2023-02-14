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

			DrawableGroup(const DrawableGroup& group) = delete;
			DrawableGroup(DrawableGroup&& group) = delete;

			DrawableGroup& operator=(const DrawableGroup& group) = delete;
			DrawableGroup& operator=(DrawableGroup&& group) = delete;

			virtual ~DrawableGroup() override = default;

		protected:

			DrawableGroup() = default;

		private:

			virtual DrawableType _getType() const override = 0;
			virtual const DrawableShaderSet& _getShaderSet() const override = 0;
			virtual const Material& _getMaterial() const override = 0;
			virtual uint32_t _getChildCount() const = 0;
			virtual const Drawable* _getChild(uint32_t index) const = 0;

			virtual void _draw(const DrawContext& context) const override = 0;


		friend class Scene;
	};
}
