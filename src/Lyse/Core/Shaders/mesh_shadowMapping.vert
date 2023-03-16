///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Vertex attributes

layout (location = 0) in vec4 va_position;
layout (location = 1) in vec4 va_normal;
layout (location = 2) in vec4 va_tangent;
layout (location = 3) in vec4 va_texCoords;

// Uniform blocks

layout (std140, row_major, binding = 2) uniform ubo_drawable_layout
{
	DrawableData ubo_drawable;
};

// Outputs

out vec2 io_vertTexCoords;

// Function definitions

void main()
{
	io_vertTexCoords = va_texCoords.xy;

	gl_Position = ubo_drawable.model * va_position;
}
