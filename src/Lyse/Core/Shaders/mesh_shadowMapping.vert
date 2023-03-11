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

// Function definitions

void main()
{
	gl_Position = ubo_drawable.model * va_position;

	// vec4 position = ubo_drawable.viewModel * va_position;
	// 
	// gl_Position = ubo_camera.projection * position;
	// gl_Position /= gl_Position.w;
	// gl_Position.z = -(2.0 * position.z + ubo_camera.far + ubo_camera.near) / (ubo_camera.far - ubo_camera.near);
}
