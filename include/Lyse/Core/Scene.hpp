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
	
			Scene(uint32_t width, uint32_t height);
			Scene(const Scene& scene) = delete;
			Scene(Scene&& scene) = delete;
	
			Scene& operator=(const Scene& scene) = delete;
			Scene& operator=(Scene&& scene) = delete;
	
			
			void resize(uint32_t width, uint32_t height);

			void setCamera(const CameraBase* camera);

			void addDrawable(const Drawable* drawable);
			void removeDrawable(const Drawable* drawable);
	

			void render() const;


			const spl::Texture2D& getTexture() const;
			bool isValid() const;


			~Scene() = default;
	
		private:

			static void _insertInDrawSequence(std::multimap<const spl::ShaderProgram*, const Drawable*>& drawSequence, const Drawable* drawable);

			static void _loadShaders();

			thread_local static uint32_t _sceneCount;
			thread_local static std::vector<spl::ShaderProgram*> _shaders;
			thread_local static std::unordered_map<DrawableType, std::unordered_map<uint64_t, spl::ShaderProgram*>> _shaderMap;
	

			spl::Framebuffer _framebuffer;

			const CameraBase* _camera;
			std::unordered_set<const Drawable*> _drawables;
	};
}
