///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Geometry layout

layout (triangles) in;
layout (triangle_strip, max_vertices = SHADOWMAP_COUNT * 3) out;

// Inputs

in vec2 io_vertTexCoords[3];

// Uniform blocks

layout (std140, row_major, binding = 3) uniform ubo_shadow_cameras_layout
{
	ShadowCamerasData ubo_shadowCameras;
};

// Uniforms

uniform float u_depthBias;

// Outputs

out vec2 io_texCoords;

// Function definitions

void main()
{
	vec4 position;
	for (int i = 0; i < ubo_shadowCameras.count; ++i)
	{
		float a = -2.0 / (ubo_shadowCameras.cameras[i].far - ubo_shadowCameras.cameras[i].near);
		float b = 0.5 * (ubo_shadowCameras.cameras[i].far + ubo_shadowCameras.cameras[i].near) * a + u_depthBias;

		for (uint j = 0; j < 3; ++j)
		{
			gl_Layer = i;

			io_texCoords = io_vertTexCoords[j];

			position = ubo_shadowCameras.cameras[i].view * gl_in[j].gl_Position;
			gl_Position = ubo_shadowCameras.cameras[i].projection * position;
			gl_Position.z = gl_Position.w * (a * position.z + b);

			EmitVertex();
		}
		
		EndPrimitive();
	}
}
