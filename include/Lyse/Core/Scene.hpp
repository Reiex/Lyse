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

			void setBackgroundFlatColor(float red, float green, float blue);
			void setBackgroundEquirectangular(const spl::Texture2D* texture);
			void setBackgroundCubemap(const spl::RawTexture* texture);	// TODO : Change that when there will a "TextureCubeMap" class in SPL

			void addDrawable(const Drawable* drawable);
			void removeDrawable(const Drawable* drawable);

			void addLight(const LightBase* light);
			void removeLight(const LightBase* light);
	

			void render() const;


			const spl::Texture2D& getDepthTexture() const;
			const spl::Texture2D& getStencilTexture() const;
			const spl::Texture2D& getColorTexture() const;
			const spl::Texture2D& getMaterialTexture() const;
			const spl::Texture2D& getNormalTexture() const;
			const spl::Texture2D& getTangentTexture() const;

			const spl::Texture2D& getSsaoTexture() const;

			const spl::Texture2D& getRenderTexture() const;

			bool isValid() const;


			~Scene() = default;


			static constexpr uint32_t maxLightCount = 128;
	
		private:

			static void _loadShaders();
			static void _insertInDrawSequence(void* pDrawSequence, const Drawable* drawable, ShaderType shaderType);
			static void _setCameraGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const CameraBase* camera);
			static void _setDrawableGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const Drawable* drawable);

			thread_local static uint32_t _sceneCount;
			thread_local static std::vector<spl::ShaderProgram*> _shaders;
			thread_local static std::unordered_map<DrawableType, std::vector<ShaderSet>> _shaderMap;


			scp::u32vec2 _resolution;

			spl::Framebuffer _gBufferFramebuffer;
			spl::Framebuffer _ssaoFramebuffer;
			spl::Framebuffer _mergeFramebuffer;

			const CameraBase* _camera;
			scp::f32vec3 _clearColor;
			const spl::RawTexture* _background;
			std::unordered_set<const Drawable*> _drawables;
			
			std::unordered_set<const LightBase*> _lights;
			mutable spl::Buffer _uboLights;
	};
}
