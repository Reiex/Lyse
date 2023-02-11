#version 460 core

in VertexOutput
{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
} io_vertexOutput;

layout (location = 0) out vec3 fo_position;
layout (location = 1) out vec3 fo_normal;
layout (location = 2) out vec3 fo_albedo;

void main()
{
    fo_position = io_vertexOutput.position;
    fo_normal = normalize(io_vertexOutput.normal);
    fo_albedo = vec3(0.5, 0.5, 0.5);
} 
