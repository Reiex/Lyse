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

// Uniforms

uniform sampler2D u_gBufferDepth;
uniform sampler2D u_gBufferNormal;
uniform sampler2D u_gBufferTangent;

uniform uint u_sampleCount;

uniform float u_scaleStep;
uniform float u_tanHalfFov;
uniform float u_twoTanHalfFov;

// Fragment outputs

layout (location = 0) out float fo_output;

// Function declarations

uint hash(in uint x);
uint hash(in const uvec2 v);
uint hash(in const uvec3 v);
uint hash(in const uvec4 v);

float random(inout uint seed);

// Function definitions

void main()
{
	uint seed = hash(floatBitsToUint(io_texCoords));
	
	const ivec2 windowCoord = ivec2(gl_FragCoord.xy);
	const float depth = ubo_camera.near + texelFetch(u_gBufferDepth, windowCoord, 0).r * ubo_camera.far;
	const vec3 viewDirUnnormalized = vec3(vec2((io_texCoords.x - 0.5) * ubo_camera.aspect, io_texCoords.y - 0.5) * u_twoTanHalfFov, -1.0);
	const vec3 position = depth * viewDirUnnormalized;
	const vec3 viewDir = normalize(viewDirUnnormalized);

	const vec3 normal = texelFetch(u_gBufferNormal, windowCoord, 0).rgb;
	const vec3 tangent = texelFetch(u_gBufferTangent, windowCoord, 0).rgb;
	const vec3 bitangent = cross(normal, tangent);
	const mat3 tbn = mat3(tangent, bitangent, normal);

	float radius = depth * u_tanHalfFov;
	
	float occlusion = 0.0;
	for (uint i = 1; i <= u_sampleCount; ++i)
	{
		// Initial vector is not uniform (cos, etc..) because more light comes from normal angle than from grazing angle
		// Scale factor is not uniform because more light comes from near diffusors than far ones...
		const float theta = random(seed) * c_2pi;
		const float phi = random(seed) * c_halfPi;
		const float cPhi = cos(phi);
		const float currentRadius = radius * i * i * u_scaleStep;
		const vec3 randomVec = vec3(cos(theta) * cPhi, sin(theta) * cPhi, sin(phi)) * currentRadius;

		const vec3 viewPos = position + tbn * randomVec;
		vec4 ndcPos = ubo_camera.projection * vec4(viewPos, 1.0);
		ndcPos.xy = (clamp(ndcPos.xy / ndcPos.w, vec2(-1.0), vec2(1.0)) + 1.0) * 0.5;

		const float sampleDepth = ubo_camera.near + texture(u_gBufferDepth, ndcPos.xy).r * ubo_camera.far;
		if (-viewPos.z > sampleDepth)
		{
			occlusion += smoothstep(0.0, 1.0, -currentRadius / (viewPos.z + sampleDepth));
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

uint hash(in const uvec2 v) { return hash( v.x ^ hash(v.y)); }
uint hash(in const uvec3 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(in const uvec4 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

float random(inout uint seed)
{
	seed = hash(seed);
	return uintBitsToFloat((seed & 0x007FFFFF) | 0x3F800000) - 1.0;
}
