#version 460 core


layout (location = 0) in vec2 va_position;

out vec2 io_texCoords;


void main()
{
    gl_Position = vec4(va_position, 0.0, 1.0);

    io_texCoords = (va_position + 1.0) / 2.0;
}
