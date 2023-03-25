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
	struct SceneParameters
	{
		spl::TextureInternalFormat depthTextureFormat = spl::TextureInternalFormat::Depth_f32;
		spl::TextureInternalFormat colorTextureFormat = spl::TextureInternalFormat::RGB_nu16;
		spl::TextureInternalFormat materialTextureFormat = spl::TextureInternalFormat::RGB_nu16;
		spl::TextureInternalFormat normalTextureFormat = spl::TextureInternalFormat::RGB_ni16;
		spl::TextureInternalFormat tangentTextureFormat = spl::TextureInternalFormat::RGB_ni16;

		bool shadowEnabled = true;
		uint32_t shadowTextureCount = 16;
		scp::u32vec2 shadowTextureResolution = { 2048, 2048 };
		spl::TextureInternalFormat shadowTextureFormat = spl::TextureInternalFormat::Depth_nu16;

		bool transparencyEnabled = true;
		spl::TextureInternalFormat transparencyColorTextureFormat = spl::TextureInternalFormat::RGBA_f32;
		spl::TextureInternalFormat transparencyCounterTextureFormat = spl::TextureInternalFormat::R_f32;

		bool ssaoEnabled = true;
		uint32_t ssaoSampleCount = 16;
		spl::TextureInternalFormat ssaoTextureFormat = spl::TextureInternalFormat::R_nu16;

		uint32_t lightSlotCount = 128;
		spl::TextureInternalFormat renderTextureFormat = spl::TextureInternalFormat::RGB_nu16;


		static constexpr uint32_t maxShadowTextureCount = 1024;
		static constexpr uint32_t maxLightSlotCount = 1024;
	};

	class Scene
	{
		public:
	
			Scene(uint32_t width, uint32_t height, const SceneParameters params = {});
			Scene(const Scene& scene) = delete;
			Scene(Scene&& scene) = delete;
	
			Scene& operator=(const Scene& scene) = delete;
			Scene& operator=(Scene&& scene) = delete;
	
			
			void resize(uint32_t width, uint32_t height);


			void setBackgroundFlatColor(float red, float green, float blue);
			void setBackgroundEquirectangular(const spl::Texture2D* texture);
			void setBackgroundCubemap(const spl::Texture* texture);	// TODO : Change that when there is a "TextureCubeMap" class in SPL

			void setCamera(const CameraBase* camera);

			void addLight(const LightBase* light);
			void removeLight(const LightBase* light);

			void addDrawable(const Drawable* drawable);
			void removeDrawable(const Drawable* drawable);
	

			void render();


			const spl::Texture2D& getDepthTexture() const;
			const spl::Texture2D& getColorTexture() const;
			const spl::Texture2D& getMaterialTexture() const;
			const spl::Texture2D& getNormalTexture() const;
			const spl::Texture2D& getTangentTexture() const;

			const spl::Texture& getShadowTexture() const;

			const spl::Texture2D& getTransparencyColorTexture() const;
			const spl::Texture2D& getTransparencyCounterTexture() const;

			const spl::Texture2D& getSsaoTexture() const;

			const spl::Texture2D& getRenderTexture() const;


			~Scene();

		private:

			void _loadShaders();

			const void _updateAndBindUboLights(uint32_t index, std::vector<const CameraBase*>& shadowCameras);
			const void _updateAndBindUboDrawable(uint32_t index, const CameraBase* camera, const Drawable* drawable);
			const void _updateAndBindUboShadowCameras(uint32_t index, const std::vector<const CameraBase*>& shadowCameras);
			void _insertInDrawSequence(void* pDrawSequence, const Drawable* drawable, ShaderType shaderType) const;

			void _setDrawableUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceDrawable* interface, const Drawable* drawable);
			void _setGBufferResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceGBufferResult* interface);
			void _setShadowResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceShadowResult* interface);
			void _setTransparencyResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceTransparencyResult* interface);
			void _setSsaoResultUniforms(const spl::ShaderProgram* shader, const shaderInterface::SubInterfaceSsaoResult* interface);

			spl::VertexArray _screenVao;
			spl::Buffer _screenVbo;


			scp::u32vec2 _resolution;
			SceneParameters _params;


			std::vector<spl::ShaderProgram*> _shaders;
			std::unordered_map<DrawableType, std::vector<ShaderSet>> _shaderMap;


			scp::f32vec3 _clearColor;
			const spl::Texture* _background;

			const CameraBase* _camera;

			std::unordered_set<const LightBase*> _lights;
			spl::Buffer _uboLights;

			std::unordered_set<const Drawable*> _drawables;
			spl::Buffer _uboDrawable;


			spl::Texture2D _depthTexture;
			spl::Texture2D _colorTexture;
			spl::Texture2D _materialTexture;
			spl::Texture2D _normalTexture;
			spl::Texture2D _tangentTexture;
			spl::Framebuffer _gBufferFramebuffer;

			spl::Texture _shadowTexture;
			spl::Framebuffer _shadowMappingFramebuffer;
			spl::Buffer _uboShadowCameras;

			spl::Texture2D _transparencyColorTexture;
			spl::Texture2D _transparencyCounterTexture;
			spl::Framebuffer _transparencyFramebuffer;

			spl::Texture2D _ssaoTexture;
			spl::Framebuffer _ssaoFramebuffer;

			spl::Texture2D _mergeTexture;
			spl::Framebuffer _mergeFramebuffer;
	};
}
