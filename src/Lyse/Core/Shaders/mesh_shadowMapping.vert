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

// Uniforms

uniform mat4 u_projection;
uniform mat4 u_viewModel;
uniform float u_near;
uniform float u_far;

// Function definitions

void main()
{
	vec4 position = u_viewModel * va_position;

	gl_Position = u_projection * position;
	gl_Position /= gl_Position.w;
	gl_Position.z = -(2.0 * position.z + u_far + u_near) / (u_far - u_near);
}
