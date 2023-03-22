///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in VertexOutput
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} io_vertexOutput;

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

#ifdef COLOR_TEXTURE
	uniform sampler2D u_drawableColor;
#else
	uniform vec4 u_drawableColor;
#endif

#ifdef MATERIAL_TEXTURE
	uniform sampler2D u_drawableMaterial;
#else
	uniform vec3 u_drawableMaterial;
#endif

#ifdef NORMAL_TEXTURE
	uniform sampler2D u_drawableNormal;
#endif

#ifdef SHADOW
	uniform sampler2DArray u_shadowTexture;
	uniform vec3 u_shadowOffset;
#endif

// Fragment outputs

layout (location = 0) out vec4 fo_color;
layout (location = 1) out float fo_counter;

// Function definitions

void main()
{
	// Compute fragment color

	#ifdef COLOR_TEXTURE
		const vec4 color = texture(u_drawableColor, io_vertexOutput.texCoords);
	#else
		const vec4 color = u_drawableColor;
	#endif

	// If there is no alpha transparency then the fragment was handled by the gBuffer shader, so it can be discarded.
	if (color.a == 1.0 || color.a == 0.0)
	{
		discard;
	}
	
	// Compute fragment material

	#ifdef MATERIAL_TEXTURE
		const vec3 material = texture(u_drawableMaterial, io_vertexOutput.texCoords).rgb;
	#else
		const vec3 material = u_drawableMaterial;
	#endif

	// Compute fragment normal

	vec3 normal = normalize(io_vertexOutput.normal);

	#ifdef NORMAL_TEXTURE
		const vec3 tangent = normalize(io_vertexOutput.tangent);
		const vec3 bitangent = cross(normal, tangent);
		const vec3 normalMapValue = normalize(texture(u_drawableNormal, io_vertexOutput.texCoords).rgb);
		normal = normalize(tangent * normalMapValue.x + bitangent * normalMapValue.y + normal * normalMapValue.z);
	#endif

	// Compute fragment final color after lighting, and store it in transparency accumulator

	fo_color.rgb = cookTorrance(color.rgb, material, normal, io_vertexOutput.position, 0.0) * color.a;
	fo_color.a = color.a;
	fo_counter = 1.0;
} 
