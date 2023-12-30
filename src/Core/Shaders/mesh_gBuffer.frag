///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in VertexOutput
{
	float depth;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} io_vertexOutput;

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

// Fragment outputs

layout (location = 0) out vec3 fo_color;
layout (location = 1) out vec3 fo_material;
layout (location = 2) out vec3 fo_normal;
layout (location = 3) out vec3 fo_tangent;

// Function definitions

void main()
{
	// Depth
	
	gl_FragDepth = io_vertexOutput.depth;

	// Color

	#ifdef COLOR_TEXTURE
		vec4 color = texture(u_drawableColor, io_vertexOutput.texCoords);
	#else
		vec4 color = u_drawableColor;
	#endif

	// If there is alpha transparency, then the fragment must be discarded. It will be handled by the transparency shader !
	if (color.a != 1.0)
	{
		discard;
	}
	
	fo_color = color.rgb;

	// Material

	#ifdef MATERIAL_TEXTURE
		fo_material = texture(u_drawableMaterial, io_vertexOutput.texCoords).rgb;
	#else
		fo_material = u_drawableMaterial;
	#endif

	// Normal and tangent

	fo_normal = normalize(io_vertexOutput.normal);
	fo_tangent = normalize(io_vertexOutput.tangent);

	#ifdef NORMAL_TEXTURE
		vec3 bitangent = cross(fo_normal, fo_tangent);
		vec3 normalMapValue = normalize(texture(u_drawableNormal, io_vertexOutput.texCoords).rgb);
		fo_normal = normalize(fo_tangent * normalMapValue.x + bitangent * normalMapValue.y + fo_normal * normalMapValue.z);
		fo_tangent = normalize(fo_tangent - dot(fo_tangent, fo_normal) * fo_normal);
	#endif
} 
