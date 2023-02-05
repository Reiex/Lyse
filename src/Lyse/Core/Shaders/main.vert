#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 pos;
out vec3 normal;

void main()
{
    normal = normalize(model * vec4(aNormal, 0.0)).xyz;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    pos = gl_Position.xyz / gl_Position.w;
}
