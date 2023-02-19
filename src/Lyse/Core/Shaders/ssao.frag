///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constants

const float c_halfPi = 1.57079632679;
const float c_2pi = 6.28318530718;

// Inputs

in vec2 io_texCoords;

// Uniforms

uniform sampler2D u_depth;
uniform sampler2D u_normal;
uniform sampler2D u_tangent;

uniform mat4 u_projection;
uniform vec4 u_cameraInfos;

uniform uint u_sampleCount;
uniform float u_radius;

// Fragment outputs

layout (location = 0) out float fo_output;

// Function declarations

uint hash(in uint x);
uint hash(in uvec2 v);
uint hash(in uvec3 v);
uint hash(in uvec4 v);

float random(inout uint seed);

// Function definitions

void main()
{
	uint seed = hash(floatBitsToUint(io_texCoords));

	float near = u_cameraInfos.x;
	float far = u_cameraInfos.y;
	const vec2 depthConversion = vec2(near * far, far - near);

	float depth = depthConversion.x / (far - texture(u_depth, io_texCoords).r * depthConversion.y);
	vec3 viewDir = normalize(vec3((io_texCoords * 2.0 - 1.0) * u_cameraInfos.zw, -1.0));
	vec3 position = viewDir * depth;

	vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);
	vec3 tangent = normalize(texture(u_tangent, io_texCoords).rgb);
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);
	
	float occlusion = 0.0;
	const float scaleStep = u_radius / (u_sampleCount * u_sampleCount);
	for (uint i = 1; i <= u_sampleCount; ++i)
	{
		float theta = random(seed) * c_2pi;
		float phi = random(seed) * c_halfPi;
		float cPhi = cos(phi);
		vec3 randomVec = vec3(cos(theta) * cPhi, sin(theta) * cPhi, sin(phi));	// Not uniform random because more light comes from normal angle than from grazing angle
		randomVec *= i * i * scaleStep;	// Not uniform distance because more light comes from near diffusors than far ones...

		vec3 viewPos = position + tbn * randomVec;
		vec4 ndcPos = u_projection * vec4(viewPos, 1.0);
		ndcPos.xy = (clamp(ndcPos.xy / ndcPos.w, vec2(-1.0), vec2(1.0)) + 1.0) * 0.5;

		float sampleDepth = depthConversion.x / (far - texture(u_depth, ndcPos.xy).r * depthConversion.y);

		if (-viewPos.z > sampleDepth)
		{
			occlusion += smoothstep(0.0, 1.0, -u_radius / (viewPos.z + sampleDepth));
		}
	}

	fo_output = 1.0 - occlusion / u_sampleCount;
} 

uint hash(in uint x)
{
	x += (x << 10);
	x ^= (x >> 6);
	x += (x << 3);
	x ^= (x >> 11);
	x += (x << 15);
	return x;
}

uint hash(in uvec2 v) { return hash( v.x ^ hash(v.y)                         ); }
uint hash(in uvec3 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash(in uvec4 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

float random(inout uint seed)
{
	seed = hash(seed);
	return uintBitsToFloat((seed & 0x007FFFFF) | 0x3F800000) - 1.0;
}
