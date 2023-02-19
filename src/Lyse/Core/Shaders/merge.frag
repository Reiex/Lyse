///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom types

struct light_t
{
	vec3 color;
	uint type;
	vec4 param0;
	vec4 param1;
};

// Constants

const float c_epsilon = 1e-5;
const float c_pi = 3.14159265359;
const vec3 c_dielectricNormalFresnelReflectance = vec3(0.04);

// Inputs

in vec2 io_texCoords;

// Uniform blocks

layout (std140, binding = 0) uniform ubo_lights_t
{
	uint count;
	light_t lights[MAX_LIGHT_COUNT];
} ubo_lights;

// Uniforms

uniform sampler2D u_depth;
uniform sampler2D u_color;
uniform sampler2D u_material;
uniform sampler2D u_normal;

uniform sampler2D u_ssao;

uniform vec2 u_texelStep;
uniform vec4 u_cameraInfos;

#ifdef BACKGROUND
	uniform mat4 u_invView;

	#ifdef BACKGROUND_PROJECTION
		uniform sampler2D u_background;
	#endif

	#ifdef BACKGROUND_CUBEMAP
		uniform samplerCube u_background;
	#endif
#endif

// Fragment outputs

layout (location = 0) out vec3 fo_output;

// Function declarations

float computeSsao();

vec2 viewDirToProjectionCoords(in vec3 viewDir);

vec3 fresnelSchlick(in vec3 normal, in vec3 lightDir, in vec3 normalFresnelReflectance);
float distributionGGX(in vec3 normal, in vec3 vector, in float roughness);
float geometryGGX(in float cTheta, in float roughness);

// Function definitions

void main()
{
	// Retrieve all data from application and precedent shader passes

	float near = u_cameraInfos.x;
	float far = u_cameraInfos.y;

	float depth = (2.0 * near * far) / (far + near - (texture(u_depth, io_texCoords).r * 2.0 - 1.0) * (far - near));
	vec3 viewDir = normalize(vec3((io_texCoords * 2.0 - 1.0) * u_cameraInfos.zw, -1.0));
	vec3 position = viewDir * depth;

	vec3 color = texture(u_color, io_texCoords).rgb;
	vec3 material = texture(u_material, io_texCoords).rgb;
	vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);

	float ssao = computeSsao();

	float ambiantCoeff = material.x;
	float metallic = material.y;
	float roughness = material.z;


	vec3 rawColor;

	// If nothing has been drawn on that fragment, show the background

	if (depth >= far * 0.999)
	{
		#ifndef BACKGROUND
			discard;
		#else
			viewDir = (u_invView * vec4(viewDir, 0)).xyz;
		
			#ifdef BACKGROUND_PROJECTION
				rawColor = texture(u_background, viewDirToProjectionCoords(viewDir)).rgb;
			#endif

			#ifdef BACKGROUND_CUBEMAP
				rawColor = texture(u_background, viewDir).rgb;
			#endif
		#endif
	}

	// Else, do lighting computations

	else
	{
		viewDir = -viewDir;
		float dotNormalViewDir = max(dot(normal, viewDir), c_epsilon);
		vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color, metallic);
		float geometryView = geometryGGX(dotNormalViewDir, roughness);

		vec3 ambiant = color * ambiantCoeff * ssao;
		vec3 diffuse = vec3(0.0, 0.0, 0.0);
		vec3 specular = vec3(0.0, 0.0, 0.0);
		for (uint i = 0; i < ubo_lights.count; ++i)
		{
			vec3 lightDir;
			vec3 radiance;

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

					float cThetaIn = ubo_lights.lights[i].param0.w;
					float cThetaOut = ubo_lights.lights[i].param1.w;
					if (cThetaIn == cThetaOut)
					{
						radiance *= float(dot(lightDir, ubo_lights.lights[i].param1.xyz) < cThetaIn);
					}
					else
					{
						float t = (clamp(dot(lightDir, ubo_lights.lights[i].param1.xyz), cThetaOut, cThetaIn) - cThetaOut) / (cThetaIn - cThetaOut);
						radiance *= exp(-pow(1.0 / t, 2.0));
					}

					break;
				}
				default:
				{
					break;
				}
			}
			
			float dotNormalLightDir = max(dot(normal, lightDir), c_epsilon);
			radiance *= dotNormalLightDir;

			if (length(radiance) < 1e-3)
			{
				continue;
			}

			vec3 halfDir = normalize(lightDir + viewDir);

			vec3 fresnelReflectance = fresnelSchlick(halfDir, lightDir, normalFresnelReflectance);
			float distribution = distributionGGX(normal, halfDir, roughness);
			float geometryLight = geometryGGX(dotNormalLightDir, roughness);

			diffuse += radiance * (vec3(1.0, 1.0, 1.0) - fresnelReflectance);
			specular += radiance * fresnelReflectance * distribution * geometryLight / dotNormalLightDir;
		}

		diffuse *= color * (1.0 - metallic) / c_pi;
		specular *= geometryView / (4.0 * dotNormalViewDir);

		rawColor = ambiant + diffuse + specular;
	}

	// Apply HDR and gamma correction to final fragment color

	rawColor = rawColor / (rawColor + 1.0);
	rawColor = pow(rawColor, vec3(1.0 / 1.2));

	fo_output = rawColor;
} 

float computeSsao()
{
	float ssao = 0.0;

	for (int i = -1; i <= 1; ++i)
	{
		for (int j = -1; j <= 1; ++j)
		{
			ssao += texture(u_ssao, io_texCoords + u_texelStep * vec2(i, j)).r;
		}
	}

	return ssao * 0.111;
}

vec2 viewDirToProjectionCoords(in vec3 viewDir)
{
	vec2 result = vec2(0.0, 0.0);

	vec2 hDir = normalize(viewDir.xz);
	result.x = atan(hDir.y, hDir.x) / (2.0 * c_pi);
	result.y = -asin(viewDir.y) / c_pi + 0.5;

	return result;
}

vec3 fresnelSchlick(in vec3 normal, in vec3 lightDir, in vec3 normalFresnelReflectance)
{
	return normalFresnelReflectance + (1.0 - normalFresnelReflectance) * pow(1.0 - max(dot(normal, lightDir), 0.0), 5);
}

float distributionGGX(in vec3 normal, in vec3 vector, in float roughness)
{
	float rSq = roughness * roughness;
	float rSqSq = rSq * rSq;
	float cTheta = max(dot(normal, vector), 0.0);
	
	float denom = (cTheta * cTheta * (rSqSq - 1.0) + 1.0);
	denom = c_pi * denom * denom;
	
	return rSqSq / denom;
}

float geometryGGX(in float cTheta, in float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	return cTheta / (cTheta * (1.0 - k) + k);
}
