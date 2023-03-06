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

layout (std140, row_major, binding = 0) uniform ubo_camera_layout
{
	CameraData ubo_camera;
};

layout (std140, row_major, binding = 2) uniform ubo_drawable_layout
{
	DrawableData ubo_drawable;
};

// Outputs

out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} io_vertexOutput;

// Function definitions

void main()
{
	io_vertexOutput.position = (ubo_drawable.viewModel * va_position).xyz;
	io_vertexOutput.normal = normalize(ubo_drawable.viewModel * va_normal).xyz;		// TODO: Change that ! Not OK for non-uniform scale !
	io_vertexOutput.tangent = normalize(ubo_drawable.viewModel * va_tangent).xyz;	// TODO: Change that ! Not OK for non-uniform scale !
	io_vertexOutput.texCoords = va_texCoords.xy;

	gl_Position = ubo_camera.projection * vec4(io_vertexOutput.position, 1.0);
}
