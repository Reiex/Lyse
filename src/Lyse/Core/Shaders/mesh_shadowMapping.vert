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

out VertexOutput
{
	vec2 texCoords;
} io_vertexOutput;

// Function definitions

void main()
{
	io_vertexOutput.texCoords = va_texCoords.xy;

	// Careful ! It's not view-space but model-space ! View-space will only be computed in geometry shader.
	gl_Position = ubo_drawable.model * va_position;
}
