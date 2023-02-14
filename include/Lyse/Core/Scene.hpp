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


			const spl::Texture2D& getDepthTexture() const;
			const spl::Texture2D& getStencilTexture() const;
			const spl::Texture2D& getColorTexture() const;
			const spl::Texture2D& getMaterialTexture() const;
			const spl::Texture2D& getPositionTexture() const;
			const spl::Texture2D& getNormalTexture() const;
			const spl::Texture2D& getTangentTexture() const;

			const spl::Texture2D& getRenderTexture() const;

			bool isValid() const;


			~Scene() = default;
	
		private:

			static void _insertInDrawSequence(void* pDrawSequence, const Drawable* drawable);

			static void _loadShaders();

			thread_local static uint32_t _sceneCount;
			thread_local static std::vector<spl::ShaderProgram*> _shaders;
			thread_local static std::unordered_map<DrawableType, std::vector<DrawableShaderSet>> _shaderMap;
	

			spl::Framebuffer _gBufferFramebuffer;
			spl::Framebuffer _mergeFramebuffer;

			const CameraBase* _camera;
			std::unordered_set<const Drawable*> _drawables;
	};
}
