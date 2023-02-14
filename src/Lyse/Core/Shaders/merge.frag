#version 460 core


in vec2 io_texCoords;

const float c_pi = 3.1415926;
const vec3 c_dielectricNormalFresnelReflectance = vec3(0.04);
const vec3 c_light = vec3(-1.0, -1.0, -1.0);

uniform sampler2D u_depth;
uniform sampler2D u_color;
uniform sampler2D u_material;
uniform sampler2D u_position;
uniform sampler2D u_normal;
// uniform sampler2D u_tangent;

uniform vec3 u_cameraPos;

layout (location = 0) out vec3 fo_output;


vec3 fresnelSchlick(in vec3 normal, in vec3 lightDir, in vec3 normalFresnelReflectance);
float distributionGGX(in vec3 normal, in vec3 halfDir, in float roughness);
float geometryGGX(in vec3 normal, in vec3 vector, in float roughness);


void main()
{
    // Retrieve all datas

    float depth = texture(u_depth, io_texCoords).r;
    vec3 color = texture(u_color, io_texCoords).rgb;
    vec3 material = texture(u_material, io_texCoords).rgb;
    vec3 position = texture(u_position, io_texCoords).rgb;
    vec3 normal = normalize(texture(u_normal, io_texCoords).rgb);

    float ambiantCoeff = material.x;
    float metallic = material.y;
    float roughness = material.z;

    // If nothign has been drawn on that fragment, shortcut the call to show the background

    if (depth == 1.0)
        discard;

    // Compute intermediate values

    vec3 lightDir = normalize(-c_light);
    vec3 viewDir = normalize(u_cameraPos - position);
    vec3 halfDir = normalize(lightDir + viewDir);

    vec3 normalFresnelReflectance = mix(c_dielectricNormalFresnelReflectance, color, metallic);

    vec3 radiance = vec3(3.0) * max(dot(normal, lightDir), 0.0);

    // Compute final "raw" color
    
    vec3 fresnelReflectance = fresnelSchlick(halfDir, lightDir, normalFresnelReflectance);
    float distribution = distributionGGX(normal, halfDir, roughness);
    float geometry = geometryGGX(normal, viewDir, roughness) * geometryGGX(normal, lightDir, roughness);

    vec3 specular = fresnelReflectance * distribution * geometry / (4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001);
    vec3 diffuse = color * (1.0 - fresnelReflectance) * (1.0 - metallic) / c_pi;
    vec3 ambiant = color * ambiantCoeff;

    vec3 finalColor = ambiant + (diffuse + specular) * radiance;

    // HDR and gamma correction

    finalColor = finalColor / (finalColor + 1.0);
    finalColor = pow(finalColor, vec3(1.0 / 1.8));

    fo_output = finalColor;
} 

vec3 fresnelSchlick(in vec3 normal, in vec3 lightDir, in vec3 normalFresnelReflectance)
{
    return normalFresnelReflectance + (1.0 - normalFresnelReflectance) * pow(1.0 - max(dot(normal, lightDir), 0.0), 5);
}

float distributionGGX(in vec3 normal, in vec3 halfDir, in float roughness)
{
    float rSq = roughness * roughness;
    float rSqSq = rSq * rSq;
    float cTheta  = max(dot(normal, halfDir), 0.0);
    float cThetaSq = cTheta * cTheta;
	
    float denom = (cThetaSq * (rSqSq - 1.0) + 1.0);
    denom = c_pi * denom * denom;
	
    return rSqSq / denom;
}

float geometryGGX(in vec3 normal, in vec3 vector, in float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    float cTheta = max(dot(normal, vector), 0.0);

    return cTheta / (cTheta * (1.0 - k) + k);
}
