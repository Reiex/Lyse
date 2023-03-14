///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constants

const vec3 c_dielectricNormalFresnelReflectance = vec3(0.04);

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

uniform sampler2D u_depth;
uniform sampler2D u_color;
uniform sampler2D u_material;
uniform sampler2D u_normal;

#ifdef SHADOW_MAPPING
	uniform sampler2DArray u_shadow;
	uniform vec3 u_shadowBlurOffset;
#endif

#ifdef SSAO
	uniform sampler2D u_ssao;
#endif

#ifdef BACKGROUND

	#ifdef BACKGROUND_PROJECTION
		uniform sampler2D u_background;
	#endif

	#ifdef BACKGROUND_CUBEMAP
		uniform samplerCube u_background;
	#endif

#endif

uniform float u_twoTanHalfFov;

// Fragment outputs

layout (location = 0) out vec3 fo_output;

// Function declarations

vec2 viewDirToBackgroundProjection(in const vec3 modelViewDir);
vec3 computeBackgroundColor(in const vec3 viewDir);


float computeSsaoValue();

float computeShadowMapValue(in const vec3 texCoords);
float computeShadowOcclusion(in const vec3 position, in const uint i);

void computeLightDirAndRadiance(in const vec3 position, in const uint i, out vec3 lightDir, out vec3 radiance);

vec3 fresnelSchlick(in const vec3 normal, in const vec3 lightDir, in const vec3 normalFresnelReflectance);
float distributionGGX(in const vec3 normal, in const vec3 vector, in const float roughness);
float geometryGGX(in const float cTheta, in const float roughness);

// Function definitions

void main()
{
	// Retrieve view-space depth, position and viewDir from fragment position and precedent passes
	
	const ivec2 windowCoord = ivec2(gl_FragCoord.xy);
	const float depth = ubo_camera.near + texelFetch(u_depth, windowCoord, 0).r * ubo_camera.far;
	const vec3 viewDirUnnormalized = vec3(vec2((io_texCoords.x - 0.5) * ubo_camera.aspect, io_texCoords.y - 0.5) * u_twoTanHalfFov, -1.0);
	const vec3 position = depth * viewDirUnnormalized;
	const vec3 viewDir = normalize(viewDirUnnormalized);

	// If nothing has been drawn on the fragment, show the background

	if (depth >= ubo_camera.far * c_oneMinusEpsilon)
	{
		#ifdef BACKGROUND
			fo_output = computeBackgroundColor(viewDir);
			return;
		#else
			discard;
		#endif
	}

	// Retrieve fragment information from precedent passes

	const vec3 color = texelFetch(u_color, windowCoord, 0).rgb;
	const vec3 material = texelFetch(u_material, windowCoord, 0).rgb;
	const vec3 normal = texelFetch(u_normal, windowCoord, 0).rgb;

	// Pre-compute useful constants

	const vec3 eyeDir = -viewDir;

	const float metallic = material.y;
	const float roughness = material.z;
	const float roughnessSq = roughness * roughness;
	const float roughnessSqSq = roughnessSq * roughnessSq;
	const float geometryConstant = roughnessSq * 0.125;
		
	const vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color, metallic);	// TODO: Check that formula...

	const float dotNormalViewDir = max(dot(normal, eyeDir), c_epsilon);
	const float geometryView = geometryGGX(dotNormalViewDir, geometryConstant);
		
	// Compute ambiant lighting

	const vec3 ambiant = color * (material.x * computeSsaoValue());

	// Iterator over each light source for diffuse and specular lighting

	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	for (uint i = 0; i < ubo_lights.count; ++i)
	{
		// If fragment is totally in shadow, skip lighting, else apply an occlusion factor

		float occlusion = computeShadowOcclusion(position, i);

		if (occlusion == 1.0)
		{
			continue;
		}

		// Compute light direction and radiance depending on light type and parameters. If light is too low, skip lighting

		vec3 lightDir, radiance;
		computeLightDirAndRadiance(position, i, lightDir, radiance);

		const float dotNormalLightDir = max(dot(normal, lightDir), c_epsilon);
		radiance *= dotNormalLightDir * (1.0 - occlusion);

		if (length(radiance) < c_epsilon)
		{
			continue;
		}

		// Finaly compute diffuse and specular lighting for this light source

		const vec3 halfDir = normalize(lightDir + eyeDir);

		const vec3 fresnelReflectance = fresnelSchlick(halfDir, lightDir, normalFresnelReflectance);
		const float distribution = distributionGGX(normal, halfDir, roughnessSqSq);
		const float geometryLight = geometryGGX(dotNormalLightDir, geometryConstant);

		diffuse += radiance * (1.0 - fresnelReflectance);
		specular += radiance * fresnelReflectance * distribution * geometryLight / dotNormalLightDir;
	}

	diffuse *= color * (1.0 - metallic) / c_pi;
	specular *= geometryView / (4.0 * dotNormalViewDir);

	vec3 rawColor = ambiant + diffuse + specular;

	// Apply HDR and gamma correction to final fragment color

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
	#ifdef BACKGROUND
		const vec3 modelViewDir = (ubo_camera.invView * vec4(viewDir, 0)).xyz;
		
		#ifdef BACKGROUND_PROJECTION
			return texture(u_background, viewDirToBackgroundProjection(modelViewDir)).rgb;
		#endif

		#ifdef BACKGROUND_CUBEMAP
			return texture(u_background, modelViewDir).rgb;
		#endif
	#else
		return vec3(0.0);
	#endif
}

float computeSsaoValue()
{
	#ifdef SSAO
		vec4 tmp = textureGather(u_ssao, io_texCoords, 0);
		float result = tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssao, io_texCoords, ivec2(-1, -1), 0);
		result += tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssao, io_texCoords, ivec2(-1, 0), 0);
		result += tmp.x + tmp.y + tmp.z + tmp.w;
		tmp = textureGatherOffset(u_ssao, io_texCoords, ivec2(0, -1), 0);
		result += tmp.x + tmp.y + tmp.z + tmp.w;
	
		return result * 0.0625;
	#else
		return 1.0;
	#endif
}

float computeShadowMapValue(in const vec3 texCoords)
{
	#ifdef SHADOW_MAPPING
		return (
			  texture(u_shadow, vec3(texCoords.x - u_shadowBlurOffset.x, texCoords.y - u_shadowBlurOffset.y, texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x - u_shadowBlurOffset.x, texCoords.y,                        texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x - u_shadowBlurOffset.x, texCoords.y + u_shadowBlurOffset.y, texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x,                        texCoords.y - u_shadowBlurOffset.y, texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x,                        texCoords.y,                        texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x,                        texCoords.y + u_shadowBlurOffset.y, texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x + u_shadowBlurOffset.x, texCoords.y - u_shadowBlurOffset.y, texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x + u_shadowBlurOffset.x, texCoords.y,                        texCoords.z)).r
			+ texture(u_shadow, vec3(texCoords.x + u_shadowBlurOffset.x, texCoords.y + u_shadowBlurOffset.y, texCoords.z)).r
		) * 0.11111;
	#else
		return 1.0;
	#endif
}

float computeShadowOcclusion(in const vec3 position, in const uint i)
{
	#ifdef SHADOW_MAPPING
		for (uint j = ubo_lights.lights[i].shadowMapStartIndex; j < ubo_lights.lights[i].shadowMapStopIndex; ++j)
		{
			vec4 shadowPosition = ubo_shadowCameras.cameras[j].view * ubo_camera.invView * vec4(position, 1.0);
			const float shadowDepth = (-shadowPosition.z - ubo_shadowCameras.cameras[j].near) / (ubo_shadowCameras.cameras[j].far - ubo_shadowCameras.cameras[j].near);
			shadowPosition = ubo_shadowCameras.cameras[j].projection * shadowPosition;
			shadowPosition.xyz /= shadowPosition.w;

			if (all(greaterThan(shadowPosition.xyz, u_shadowBlurOffset - vec3(1.0))) && all(lessThan(shadowPosition.xyz, vec3(1.0) - u_shadowBlurOffset)))
			{
				const float sampledShadowDepth = computeShadowMapValue(vec3(shadowPosition.xy * 0.5 + 0.5, j));
				return shadowDepth > sampledShadowDepth ? 1.0 : 0.0;
			}
		}
	#endif

	return 0.0;
}

void computeLightDirAndRadiance(in const vec3 position, in const uint i, out vec3 lightDir, out vec3 radiance)
{
	switch (ubo_lights.lights[i].type)
	{
		case 0:		// Point
		{
			lightDir = ubo_lights.lights[i].param0.xyz - position;
			radiance = ubo_lights.lights[i].color / max(dot(lightDir, lightDir), c_epsilon);
			lightDir = normalize(lightDir);
			break;
		}
		case 1:		// Sun
		{
			lightDir = ubo_lights.lights[i].param0.xyz;
			radiance = ubo_lights.lights[i].color;
			break;
		}
		case 2:		// Spot
		{
			lightDir = ubo_lights.lights[i].param0.xyz - position;
			radiance = ubo_lights.lights[i].color / max(dot(lightDir, lightDir), c_epsilon);
			lightDir = normalize(lightDir);

			const float cThetaIn = ubo_lights.lights[i].param0.w;
			const float cThetaOut = ubo_lights.lights[i].param1.w;
			if (cThetaIn == cThetaOut)
			{
				radiance *= float(dot(lightDir, ubo_lights.lights[i].param1.xyz) < cThetaIn);
			}
			else
			{
				const float t = (clamp(dot(lightDir, ubo_lights.lights[i].param1.xyz), cThetaOut, cThetaIn) - cThetaOut) / (cThetaIn - cThetaOut);
				radiance *= exp(-pow(1.0 / t, 2.0));
			}

			break;
		}
		default:
		{
			break;
		}
	}
}

vec3 fresnelSchlick(in const vec3 normal, in const vec3 lightDir, in const vec3 normalFresnelReflectance)
{
	return normalFresnelReflectance + (1.0 - normalFresnelReflectance) * pow(1.0 - max(dot(normal, lightDir), 0.0), 5);
}

float distributionGGX(in const vec3 normal, in const vec3 vector, in const float roughnessSqSq)
{
	const float cTheta = max(dot(normal, vector), 0.0);
	
	float denom = (cTheta * cTheta * (roughnessSqSq - 1.0) + 1.0);
	denom = c_pi * denom * denom;
	
	return roughnessSqSq / denom;
}

float geometryGGX(in const float cTheta, in const float geometryConstant)
{
	return cTheta / mix(cTheta, 1.0, geometryConstant);
}
