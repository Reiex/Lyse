///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


layout (location = 0) in vec4 va_position;
layout (location = 1) in vec4 va_normal;
layout (location = 2) in vec4 va_tangent;
layout (location = 3) in vec4 va_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

out VertexOutput
{
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texCoords;
} io_vertexOutput;


void main()
{
	gl_Position = u_projection * u_view * u_model * va_position;

	io_vertexOutput.position = (u_model * va_position).xyz;
	io_vertexOutput.normal = normalize(u_model * va_normal).xyz;
	io_vertexOutput.tangent = normalize(u_model * va_tangent).xyz;
	io_vertexOutput.texCoords = va_texCoords.xy;
}
