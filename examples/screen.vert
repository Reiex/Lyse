#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 3) in vec4 aTexCoords;

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords.xy;
    gl_Position = aPos;
}
