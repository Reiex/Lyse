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
	UboCameraData ubo_camera;
};


// Uniforms

uniform sampler2D u_depth;
uniform sampler2D u_normal;
uniform sampler2D u_tangent;

uniform uint u_sampleCount;
uniform float u_radius;
uniform float u_scaleStep;

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
	
	const float depth = cameraComputeDepth(ubo_camera, u_depth, io_texCoords);
	const vec3 viewDirUnnormalized = cameraComputeUnnormalizedViewDir(ubo_camera, io_texCoords, depth);
	const vec3 position = cameraComputePosition(ubo_camera, io_texCoords, depth, viewDirUnnormalized);
	const vec3 viewDir = normalize(viewDirUnnormalized);

	const vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);
	const vec3 tangent = normalize(texture(u_tangent, io_texCoords).rgb);
	const vec3 bitangent = cross(normal, tangent);
	const mat3 tbn = mat3(tangent, bitangent, normal);
	
	float occlusion = 0.0;
	for (uint i = 1; i <= u_sampleCount; ++i)
	{
		const float theta = random(seed) * c_2pi;
		const float phi = random(seed) * c_halfPi;
		const float cPhi = cos(phi);
		// Initial vector is not uniform (cos, etc..) because more light comes from normal angle than from grazing angle
		// Scale factor is not uniform because more light comes from near diffusors than far ones...
		const vec3 randomVec = vec3(cos(theta) * cPhi, sin(theta) * cPhi, sin(phi)) * i * i * u_scaleStep;

		const vec3 viewPos = position + tbn * randomVec;
		vec4 ndcPos = ubo_camera.projection * vec4(viewPos, 1.0);
		ndcPos.xy = (clamp(ndcPos.xy / ndcPos.w, vec2(-1.0), vec2(1.0)) + 1.0) * 0.5;

		const float sampleDepth = cameraComputeDepth(ubo_camera, u_depth, ndcPos.xy);
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

uint hash(in const uvec2 v) { return hash( v.x ^ hash(v.y)); }
uint hash(in const uvec3 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z)); }
uint hash(in const uvec4 v) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w)); }

float random(inout uint seed)
{
	seed = hash(seed);
	return uintBitsToFloat((seed & 0x007FFFFF) | 0x3F800000) - 1.0;
}
