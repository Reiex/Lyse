#version 460 core


in VertexOutput
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} io_vertexOutput;

uniform vec3 u_color;
uniform vec3 u_material;

layout (location = 0) out vec3 fo_color;
layout (location = 1) out vec3 fo_material;
layout (location = 2) out vec3 fo_position;
layout (location = 3) out vec3 fo_normal;
layout (location = 4) out vec3 fo_tangent;


void main()
{
    fo_color = u_color;
    fo_material = u_material;
    fo_position = io_vertexOutput.position;
    fo_normal = normalize(io_vertexOutput.normal);
    fo_tangent = normalize(io_vertexOutput.tangent);
} 
