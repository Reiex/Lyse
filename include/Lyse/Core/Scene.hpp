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
	class Scene
	{
		public:
	
			// TODO: Handle resize (resize framebuffer..)
			Scene(uint32_t width, uint32_t height);
			Scene(const Scene& scene) = delete;
			Scene(Scene&& scene) = delete;
	
			Scene& operator=(const Scene& scene) = delete;
			Scene& operator=(Scene&& scene) = delete;
	

			void setCamera(const CameraBase* camera);

			void addDrawable(const SceneDrawable* drawable);
			void removeDrawable(const SceneDrawable* drawable);
	

			void render() const;
			const spl::Texture2D* getTexture() const;


			bool isValid() const;


			~Scene() = default;
	
		private:
	
			spl::Framebuffer _framebuffer;
			spl::ShaderProgram _shader;

			const CameraBase* _camera;
			std::unordered_set<const SceneDrawable*> _drawables;
	};
}
