///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom types

struct LightData
{
	vec3 color;
	uint type;
	vec4 param0;
	vec4 param1;
};

struct LightsData
{
	uint count;
	LightData lights[MAX_LIGHT_COUNT];
};

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

// Uniforms

uniform sampler2D u_depth;
uniform sampler2D u_color;
uniform sampler2D u_material;
uniform sampler2D u_normal;

uniform sampler2D u_shadow;
uniform mat4 u_lightProj;
uniform mat4 u_lightView;
uniform float u_lightNear;
uniform float u_lightFar;

uniform sampler2D u_ssao;

#ifdef BACKGROUND

	#ifdef BACKGROUND_PROJECTION
		uniform sampler2D u_background;
	#endif

	#ifdef BACKGROUND_CUBEMAP
		uniform samplerCube u_background;
	#endif

#endif

uniform float u_tanHalfFov;
uniform vec2 u_blurOffset;

// Fragment outputs

layout (location = 0) out vec3 fo_output;

// Function declarations

float computeSsao();

vec2 viewDirToProjectionCoords(in const vec3 viewDir);

vec3 fresnelSchlick(in const vec3 normal, in const vec3 lightDir, in const vec3 normalFresnelReflectance);
float distributionGGX(in const vec3 normal, in const vec3 vector, in const float roughness);
float geometryGGX(in const float cTheta, in const float roughness);

// Function definitions

void main()
{
	// Retrieve all data from application and precedent shader passes
	
	const float depth = ubo_camera.near + texture(u_depth, io_texCoords).r * ubo_camera.far;
	const vec3 viewDirUnnormalized = vec3(vec2((io_texCoords.x - 0.5) * ubo_camera.aspect, io_texCoords.y - 0.5) * (u_tanHalfFov * 2.0), -1.0);
	const vec3 position = depth * viewDirUnnormalized;
	const vec3 viewDir = normalize(viewDirUnnormalized);

	const vec3 color = texture(u_color, io_texCoords).rgb;
	const vec3 material = texture(u_material, io_texCoords).rgb;
	const vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);

	const float ssao = computeSsao();

	const float ambiantCoeff = material.x;
	const float metallic = material.y;
	const float roughness = material.z;
	const float roughnessSq = roughness * roughness;
	const float roughnessSqSq = roughnessSq * roughnessSq;
	const float geometryConstant = roughnessSq * 0.125;


	vec3 rawColor;

	// If nothing has been drawn on that fragment, show the background

	if (depth >= ubo_camera.far * 0.999)
	{
		#ifndef BACKGROUND
			discard;
		#else
			const vec3 modelViewDir = (ubo_camera.invView * vec4(viewDir, 0)).xyz;
		
			#ifdef BACKGROUND_PROJECTION
				rawColor = texture(u_background, viewDirToProjectionCoords(modelViewDir)).rgb;
			#endif

			#ifdef BACKGROUND_CUBEMAP
				rawColor = texture(u_background, modelViewDir).rgb;
			#endif
		#endif
	}

	// Else, do lighting computations

	else
	{
		const vec3 nViewDir = -viewDir;
		const float dotNormalViewDir = max(dot(normal, nViewDir), c_epsilon);
		const vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color, metallic);
		const float geometryView = geometryGGX(dotNormalViewDir, roughness);

		const vec3 ambiant = color * ambiantCoeff * ssao;
		vec3 diffuse = vec3(0.0, 0.0, 0.0);
		vec3 specular = vec3(0.0, 0.0, 0.0);
		for (uint i = 0; i < ubo_lights.count; ++i)
		{
			if (i == 0)
			{
				vec4 p = u_lightView * ubo_camera.invView * vec4(position, 1.0);
				float d = -(p.z + u_lightNear) / (u_lightFar - u_lightNear);
				p = u_lightProj * p;
				p.xy /= p.w;
			
				if (d > texture(u_shadow, p.xy * 0.5 + 0.5).r + 0.001)
				{
					continue;
				}
			}

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
			
			const float dotNormalLightDir = max(dot(normal, lightDir), c_epsilon);
			radiance *= dotNormalLightDir;

			if (length(radiance) < 1e-3)
			{
				continue;
			}

			const vec3 halfDir = normalize(lightDir + nViewDir);

			const vec3 fresnelReflectance = fresnelSchlick(halfDir, lightDir, normalFresnelReflectance);
			const float distribution = distributionGGX(normal, halfDir, roughnessSqSq);
			const float geometryLight = geometryGGX(dotNormalLightDir, geometryConstant);

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
	return (
		  texture(u_ssao, vec2(io_texCoords.x - u_blurOffset.x, io_texCoords.y - u_blurOffset.y)).r
		+ texture(u_ssao, vec2(io_texCoords.x - u_blurOffset.x, io_texCoords.y                 )).r
		+ texture(u_ssao, vec2(io_texCoords.x - u_blurOffset.x, io_texCoords.y + u_blurOffset.y)).r
		+ texture(u_ssao, vec2(io_texCoords.x,                  io_texCoords.y - u_blurOffset.y)).r
		+ texture(u_ssao, vec2(io_texCoords.x,                  io_texCoords.y                 )).r
		+ texture(u_ssao, vec2(io_texCoords.x,                  io_texCoords.y + u_blurOffset.y)).r
		+ texture(u_ssao, vec2(io_texCoords.x + u_blurOffset.x, io_texCoords.y - u_blurOffset.y)).r
		+ texture(u_ssao, vec2(io_texCoords.x + u_blurOffset.x, io_texCoords.y                 )).r
		+ texture(u_ssao, vec2(io_texCoords.x + u_blurOffset.x, io_texCoords.y + u_blurOffset.y)).r
	) * 0.111111;
}

vec2 viewDirToProjectionCoords(in const vec3 viewDir)
{
	vec2 result = vec2(0.0, 0.0);

	vec2 hDir = normalize(viewDir.xz);
	result.x = atan(hDir.y, hDir.x) * c_inv2pi;
	result.y = -asin(viewDir.y) * c_invPi + 0.5;

	return result;
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
