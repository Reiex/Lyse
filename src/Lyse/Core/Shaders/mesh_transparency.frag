///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constants

const vec3 c_dielectricNormalFresnelReflectance = vec3(0.04);

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
	uniform vec3 u_shadowTextureBlurOffset;
#endif

// Fragment outputs

layout (location = 0) out vec4 fo_color;
layout (location = 1) out float fo_counter;

// Function declarations

float computeShadowMapValue(in const vec3 texCoords);
float computeShadowOcclusion(in const vec3 position, in const uint i);

void computeLightDirAndRadiance(in const vec3 position, in const uint i, out vec3 lightDir, out vec3 radiance);

vec3 fresnelSchlick(in const vec3 normal, in const vec3 lightDir, in const vec3 normalFresnelReflectance);
float distributionGGX(in const vec3 normal, in const vec3 vector, in const float roughness);
float geometryGGX(in const float cTheta, in const float roughness);

// Function definitions

void main()
{
	// Color

	#ifdef COLOR_TEXTURE
		const vec4 color = texture(u_drawableColor, io_vertexOutput.texCoords);
	#else
		const vec4 color = u_drawableColor;
	#endif

	if (color.a == 1.0 || color.a == 0.0)
	{
		discard;
	}
	
	// Material

	#ifdef MATERIAL_TEXTURE
		const vec3 material = texture(u_drawableMaterial, io_vertexOutput.texCoords).rgb;
	#else
		const vec3 material = u_drawableMaterial;
	#endif

	// Normal

	vec3 normal = normalize(io_vertexOutput.normal);

	#ifdef NORMAL_TEXTURE
		const vec3 tangent = normalize(io_vertexOutput.tangent);
		const vec3 bitangent = cross(normal, tangent);
		const vec3 normalMapValue = normalize(texture(u_drawableNormal, io_vertexOutput.texCoords).rgb);
		normal = normalize(tangent * normalMapValue.x + bitangent * normalMapValue.y + normal * normalMapValue.z);
	#endif

	// Pre-compute useful constants

	const vec3 position = io_vertexOutput.position;
	const vec3 eyeDir = -normalize(position);

	const float metallic = material.y;
	const float roughness = material.z;
	const float roughnessSq = roughness * roughness;
	const float roughnessSqSq = roughnessSq * roughnessSq;
	const float geometryConstant = roughnessSq * 0.125;
		
	const vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color.rgb, metallic);	// TODO: Check that formula...

	const float dotNormalViewDir = max(dot(normal, eyeDir), c_epsilon);
	const float geometryView = geometryGGX(dotNormalViewDir, geometryConstant);
		
	// Compute ambiant lighting

	const vec3 ambiant = color.rgb * material.x;

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

	diffuse *= color.rgb * (1.0 - metallic) / c_pi;
	specular *= geometryView / (4.0 * dotNormalViewDir);

	fo_color.rgb = (ambiant + diffuse + specular) * color.a;
	fo_color.a = color.a;
	fo_counter = 1.0;
} 

float computeShadowMapValue(in const vec3 texCoords)
{
	#ifdef SHADOW
		return (
			  texture(u_shadowTexture, vec3(texCoords.x - u_shadowTextureBlurOffset.x, texCoords.y - u_shadowTextureBlurOffset.y, texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x - u_shadowTextureBlurOffset.x, texCoords.y,                               texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x - u_shadowTextureBlurOffset.x, texCoords.y + u_shadowTextureBlurOffset.y, texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x,                               texCoords.y - u_shadowTextureBlurOffset.y, texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x,                               texCoords.y,                               texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x,                               texCoords.y + u_shadowTextureBlurOffset.y, texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x + u_shadowTextureBlurOffset.x, texCoords.y - u_shadowTextureBlurOffset.y, texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x + u_shadowTextureBlurOffset.x, texCoords.y,                               texCoords.z)).r
			+ texture(u_shadowTexture, vec3(texCoords.x + u_shadowTextureBlurOffset.x, texCoords.y + u_shadowTextureBlurOffset.y, texCoords.z)).r
		) * 0.11111;
	#else
		return 1.0;
	#endif
}

float computeShadowOcclusion(in const vec3 position, in const uint i)
{
	#ifdef SHADOW
		for (uint j = ubo_lights.lights[i].shadowMapStartIndex; j < ubo_lights.lights[i].shadowMapStopIndex; ++j)
		{
			vec4 shadowPosition = ubo_shadowCameras.cameras[j].view * ubo_camera.invView * vec4(position, 1.0);
			const float shadowDepth = (-shadowPosition.z - ubo_shadowCameras.cameras[j].near) / (ubo_shadowCameras.cameras[j].far - ubo_shadowCameras.cameras[j].near);
			shadowPosition = ubo_shadowCameras.cameras[j].projection * shadowPosition;
			shadowPosition.xyz /= shadowPosition.w;

			if (all(greaterThan(shadowPosition.xyz, u_shadowTextureBlurOffset - vec3(1.0))) && all(lessThan(shadowPosition.xyz, vec3(1.0) - u_shadowTextureBlurOffset)))
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
