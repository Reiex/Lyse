///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function definitions

float computeShadowOcclusion(in const vec3 position, in const vec3 normal, in const uint i)
{
	#ifdef SHADOW
		// For each shadow texture associated with the i-th light

		for (uint j = ubo_lights.lights[i].shadowMapStartIndex; j < ubo_lights.lights[i].shadowMapStopIndex; ++j)
		{
			// Compute fragment position in shadow-view-space coordinate system (linear depth and projected xyz)

			vec4 shadowPosition = ubo_shadowCameras.cameras[j].view * ubo_camera.invView * vec4(position + (sqrt(-position.z) * 5e-3) * normal, 1.0);
			const float depth = (-shadowPosition.z - ubo_shadowCameras.cameras[j].near) / (ubo_shadowCameras.cameras[j].far - ubo_shadowCameras.cameras[j].near);
			shadowPosition = ubo_shadowCameras.cameras[j].projection * shadowPosition;
			shadowPosition.xyz /= shadowPosition.w;

			// If the fragment position is inside the "shadow frustum" compute and return occlusion

			if (all(greaterThan(shadowPosition.xyz, u_shadowOffset - vec3(1.0))) && all(lessThan(shadowPosition.xyz, vec3(1.0) - u_shadowOffset)))
			{
				const vec4 texCoords = vec4(shadowPosition.xy * 0.5 + 0.5, j, depth);
				return texture(u_shadowTexture, texCoords);
			}
		}
	#endif

	// If shadow is disabled or fragment is not in any shadow frustum, there is simply no occlusion

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

vec3 cookTorrance(in const vec3 color, in const vec3 material, in const vec3 normal, in const vec3 position, in float ssao)
{
	// Pre-compute useful constants

	const vec3 eyeDir = -normalize(position);

	const float metallic = material.y;
	const float roughness = material.z;
	const float roughnessSq = roughness * roughness;
	const float roughnessSqSq = roughnessSq * roughnessSq;
	const float geometryConstant = roughnessSq * 0.125;
		
	const vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color, metallic);	// TODO: Check that formula...

	const float dotNormalViewDir = max(dot(normal, eyeDir), c_epsilon);
	const float geometryView = geometryGGX(dotNormalViewDir, geometryConstant);
		
	// Compute ambiant lighting

	const vec3 ambiant = color * (material.x * (1.0 - ssao));

	// Iterate over each light source for diffuse and specular lighting

	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);
	for (uint i = 0; i < ubo_lights.count; ++i)
	{
		// If fragment is totally in shadow, skip lighting with this light, else apply an occlusion factor

		float occlusion = computeShadowOcclusion(position, normal, i);

		if (occlusion == 1.0)
		{
			continue;
		}

		// Compute light direction and radiance depending on light type and parameters. If light is too weak, skip lighting

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

	// Correct diffuse and specular by some constants (computed at the end for optimization)

	diffuse *= color * (1.0 - metallic) / c_pi;
	specular *= geometryView / (4.0 * dotNormalViewDir);

	return ambiant + diffuse + specular;
}
