///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom types 

struct UboCameraData
{
	mat4 projection;
	uvec2 resolution;
	float near;
	float far;
	float aspect;
	float fov;

	float tanHalfFov;
	vec2 texelSize;
	vec2 depthConversion;
};

// Constants

const float c_epsilon = 1e-5;
const float c_halfPi = 1.57079632679;
const float c_pi = 3.14159265359;
const float c_2pi = 6.28318530718;

// Function declarations

float cameraComputeDepth(in const UboCameraData cameraData, in const sampler2D depthMap, in const vec2 texCoords);
vec3 cameraComputeUnnormalizedViewDir(in const UboCameraData cameraData, in const vec2 texCoords, in const float depth);
vec3 cameraComputePosition(in const UboCameraData cameraData, in const vec2 texCoords, in const float depth, in const vec3 unnormaliedViewDir);

// Function definitions

float cameraComputeDepth(in const UboCameraData cameraData, in const sampler2D depthMap, in const vec2 texCoords)
{
	#ifdef CAMERA_PERSPECTIVE
		return cameraData.depthConversion.x / (cameraData.far - texture(depthMap, texCoords).r * cameraData.depthConversion.y);
	#endif

	#ifdef CAMERA_ORTHOGRAPHIC
		return texture(depthMap, texCoords).r * cameraData.far;
	#endif

	return 0.0;
}

vec3 cameraComputeUnnormalizedViewDir(in const UboCameraData cameraData, in const vec2 texCoords, in const float depth)
{
	#ifdef CAMERA_PERSPECTIVE
		return vec3(vec2((texCoords.x - 0.5) * cameraData.aspect, texCoords.y - 0.5) * (cameraData.tanHalfFov * 2.0), -1.0);
	#endif

	#ifdef CAMERA_ORTHOGRAPHIC
		return vec3(0.0, 0.0, -1.0);
	#endif

	return vec3(0.0, 0.0, -1.0);
}

vec3 cameraComputePosition(in const UboCameraData cameraData, in const vec2 texCoords, in const float depth, in const vec3 unnormaliedViewDir)
{
	#ifdef CAMERA_PERSPECTIVE
		return depth * unnormaliedViewDir;
	#endif

	#ifdef CAMERA_ORTHOGRAPHIC
		return vec3(vec2((texCoords.x - 0.5) * cameraData.aspect, texCoords.y - 0.5) * (2.0 * cameraData.tanHalfFov * depth), -depth);
	#endif

	return vec3(0.0, 0.0, 0.0);
}
