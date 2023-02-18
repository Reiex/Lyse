///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in vec2 io_texCoords;

// Uniforms

uniform sampler2D u_depth;
uniform sampler2D u_normal;
uniform sampler2D u_tangent;

uniform mat4 u_projection;
uniform vec4 u_cameraInfos;

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

	float depth = (2.0 * near * far) / (far + near - (texture(u_depth, io_texCoords).r * 2.0 - 1.0) * (far - near));
	vec3 viewDir = normalize(vec3((io_texCoords * 2.0 - 1.0) * u_cameraInfos.zw, -1.0));
	vec3 position = viewDir * depth;

	vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);
	vec3 tangent = normalize(texture(u_tangent, io_texCoords).rgb);
	vec3 bitangent = cross(normal, tangent);
	mat3 tbn = mat3(tangent, bitangent, normal);

	uint u_ssaoSamples = 16;

	uint count = 0;
	for (uint i = 0; i < u_ssaoSamples; ++i)
	{
		vec3 randomVec = vec3(2.0 * random(seed) - 1.0, 2.0 * random(seed) - 1.0, random(seed));	// TODO : Direct random vector ?
		vec3 viewPos = position + tbn * randomVec * 0.1;	// TODO: Fix magic number (poisson distribution ?)
		vec4 ndcPos = u_projection * vec4(viewPos, 1.0);
		ndcPos.xyz = (clamp(ndcPos.xyz / ndcPos.w, vec3(-1.0), vec3(1.0)) + 1.0) * 0.5;

		if (ndcPos.z < texture(u_depth, ndcPos.xy).r)
		{
			++count;
		}
	}

	fo_output = float(count) / float(u_ssaoSamples - 1);
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
