///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in vec2 io_texCoords;

// Uniform blocks

layout (std140, row_major, binding = 0) uniform ubo_camera_layout
{
	CameraData ubo_camera;
};

layout (std140, row_major, binding = 1) uniform ubo_lights_layout
{
	LightsData ubo_lights;
};

layout (std140, row_major, binding = 3) uniform ubo_shadow_cameras_layout
{
	ShadowCamerasData ubo_shadowCameras;
};

// Uniforms

uniform sampler2D u_gBufferDepth;
uniform sampler2D u_gBufferColor;
uniform sampler2D u_gBufferMaterial;
uniform sampler2D u_gBufferNormal;

#ifdef SHADOW
	uniform sampler2DArrayShadow u_shadowTexture;
	uniform vec3 u_shadowOffset;
#endif

#ifdef TRANSPARENCY
	uniform sampler2D u_transparencyColor;
	uniform sampler2D u_transparencyCounter;
#endif

#ifdef SSAO
	uniform sampler2D u_ssaoTexture;
#endif

#ifdef BACKGROUND_TEXTURE
	#ifdef BACKGROUND_PROJECTION
		uniform sampler2D u_background;
	#endif
	#ifdef BACKGROUND_CUBEMAP
		uniform samplerCube u_background;
	#endif
#else
	uniform vec3 u_background;
#endif

uniform float u_twoTanHalfFov;

// Fragment outputs

layout (location = 0) out vec3 fo_output;

// Function declarations

vec2 viewDirToBackgroundProjection(in const vec3 modelViewDir);
vec3 computeBackgroundColor(in const vec3 viewDir);

float computeSsao();

// Function definitions

void main()
{
	// Retrieve view-space depth, position and viewDir from fragment position and precedent passes
	
	const ivec2 windowCoord = ivec2(gl_FragCoord.xy);
	const float depth = ubo_camera.near + texelFetch(u_gBufferDepth, windowCoord, 0).r * ubo_camera.far;
	const vec3 viewDirUnnormalized = vec3(vec2((io_texCoords.x - 0.5) * ubo_camera.aspect, io_texCoords.y - 0.5) * u_twoTanHalfFov, -1.0);
	const vec3 position = depth * viewDirUnnormalized;
	const vec3 viewDir = normalize(viewDirUnnormalized);

	vec3 rawColor;

	// If no opaque fragment has been drawn, show the background

	if (depth >= ubo_camera.far * c_oneMinusEpsilon)
	{
		rawColor = computeBackgroundColor(viewDir);
	}
	
	// Else compute the opaque fragment lighting

	else
	{
		const vec3 color = texelFetch(u_gBufferColor, windowCoord, 0).rgb;
		const vec3 material = texelFetch(u_gBufferMaterial, windowCoord, 0).rgb;
		const vec3 normal = texelFetch(u_gBufferNormal, windowCoord, 0).rgb;

		rawColor = cookTorrance(color, material, normal, position, computeSsao());
	}

	// Draw transparency fragments over the opaque fragment if there are

	#ifdef TRANSPARENCY
		float n = texelFetch(u_transparencyCounter, windowCoord, 0).r;
		if (n != 0.0)
		{
			vec4 transparencyColor = texelFetch(u_transparencyColor, windowCoord, 0);
			rawColor = (transparencyColor.rgb / transparencyColor.a) * (1.0 - pow(1.0 - transparencyColor.a / n, n)) + rawColor * pow(1.0 - transparencyColor.a / n, n);
		}
	#endif

	// Apply HDR and gamma correction to the final fragment color

	rawColor = rawColor / (rawColor + 0.5);
	rawColor = pow(rawColor, vec3(1.0 / 1.2));

	fo_output = rawColor;
}

vec2 viewDirToBackgroundProjection(in const vec3 modelViewDir)
{
	const vec2 hDir = normalize(modelViewDir.xz);
	return vec2(atan(hDir.y, hDir.x) * c_inv2pi, asin(modelViewDir.y) * c_invPi + 0.5);
}

vec3 computeBackgroundColor(in const vec3 viewDir)
{
	#ifdef BACKGROUND_TEXTURE
		const vec3 modelViewDir = (ubo_camera.invView * vec4(viewDir, 0)).xyz;
		
		#ifdef BACKGROUND_PROJECTION
			return texture(u_background, viewDirToBackgroundProjection(modelViewDir)).rgb;
		#endif

		#ifdef BACKGROUND_CUBEMAP
			return texture(u_background, modelViewDir).rgb;
		#endif
	#else
		return u_background;
	#endif
}

float computeSsao()
{
	#ifdef SSAO
		vec4 tmp = textureGather(u_ssaoTexture, io_texCoords, 0);
		float occlusion = tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssaoTexture, io_texCoords, ivec2(-1, -1), 0);
		occlusion += tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssaoTexture, io_texCoords, ivec2(-1, 0), 0);
		occlusion += tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssaoTexture, io_texCoords, ivec2(0, -1), 0);
		return (occlusion + tmp.x + tmp.y + tmp.z + tmp.w) * 0.0625;
	#else
		return 1.0;
	#endif
}
