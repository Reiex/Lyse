#version 460 core

layout (location = 0) in vec3 va_position;
layout (location = 1) in vec3 va_normal;
layout (location = 2) in vec2 va_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out VertexOutput
{
    vec3 position;
    vec3 normal;
    vec2 texCoords;
} io_vertexOutput;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(va_position, 1.0);

    io_vertexOutput.position = (u_model * vec4(va_position, 1.0)).xyz;
    io_vertexOutput.normal = normalize(u_model * vec4(va_normal, 0.0)).xyz;
    io_vertexOutput.texCoords = va_texCoords;
}
