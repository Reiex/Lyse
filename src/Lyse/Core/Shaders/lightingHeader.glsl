///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constants

const vec3 c_dielectricNormalFresnelReflectance = vec3(0.04);

// Function declarations

float computeShadowOcclusion(in const vec3 position, in const vec3 normal, in const uint i);
void computeLightDirAndRadiance(in const vec3 position, in const uint i, out vec3 lightDir, out vec3 radiance);

vec3 fresnelSchlick(in const vec3 normal, in const vec3 lightDir, in const vec3 normalFresnelReflectance);
float distributionGGX(in const vec3 normal, in const vec3 vector, in const float roughness);
float geometryGGX(in const float cTheta, in const float roughness);

vec3 cookTorrance(in const vec3 color, in const vec3 material, in const vec3 normal, in const vec3 position, in float ssao);
