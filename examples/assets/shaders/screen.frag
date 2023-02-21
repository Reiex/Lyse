#version 330 core

in vec2 texCoords;

uniform sampler2D scene;

out vec4 FragColor;

void main()
{
    FragColor = texture(scene, texCoords);
}
