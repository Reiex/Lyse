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

			void setBackgroundFlatColor(float red, float green, float blue);
			void setBackgroundEquirectangular(const spl::Texture2D* texture);
			void setBackgroundCubemap(const spl::RawTexture* texture);	// TODO : Change that when there will a "TextureCubeMap" class in SPL

			void setCamera(const CameraBase* camera);

			void addLight(const LightBase* light);
			void removeLight(const LightBase* light);

			void addDrawable(const Drawable* drawable);
			void removeDrawable(const Drawable* drawable);
	

			void render() const;


			const spl::Texture2D& getDepthTexture() const;
			const spl::Texture2D& getStencilTexture() const;
			const spl::Texture2D& getColorTexture() const;
			const spl::Texture2D& getMaterialTexture() const;
			const spl::Texture2D& getNormalTexture() const;
			const spl::Texture2D& getTangentTexture() const;

			const spl::Texture2D& getShadowMap() const;

			const spl::Texture2D& getSsaoTexture() const;

			const spl::Texture2D& getRenderTexture() const;

			bool isValid() const;


			~Scene();


			static constexpr uint32_t maxLightCount = 128;
	
		private:

			void _loadShaders();

			const void* _updateUboCamera() const;
			const void* _updateUboLights() const;
			const void* _updateUboDrawable(const void* pUboCameraData, const Drawable* drawable) const;
			void _insertInDrawSequence(void* pDrawSequence, const Drawable* drawable, ShaderType shaderType) const;

			static void _setGBufferUniforms(const std::pair<const spl::ShaderProgram*, const GBufferShaderInterface*>& gBuffer, const Drawable* drawable);


			std::vector<spl::ShaderProgram*> _shaders;
			std::unordered_map<DrawableType, std::vector<ShaderSet>> _shaderMap;

			spl::Framebuffer _gBufferFramebuffer;
			spl::Framebuffer _shadowMappingFramebuffer;
			spl::Framebuffer _ssaoFramebuffer;
			spl::Framebuffer _mergeFramebuffer;

			scp::f32vec3 _clearColor;
			const spl::RawTexture* _background;

			scp::u32vec2 _resolution;
			const CameraBase* _camera;
			mutable spl::Buffer _uboCamera;
			
			std::unordered_set<const LightBase*> _lights;
			mutable spl::Buffer _uboLights;

			std::unordered_set<const Drawable*> _drawables;
			mutable spl::Buffer _uboDrawable;

			spl::VertexArray _screenVao;
			spl::Buffer _screenVbo;
	};
}
