///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Geometry layout

layout (triangles) in;
layout (triangle_strip, max_vertices = SHADOW_TEXTURE_COUNT * 3) out;

// Inputs

in VertexOutput
{
	vec2 texCoords;
} io_vertexOutput[3];

// Uniform blocks

layout (std140, row_major, binding = 3) uniform ubo_shadow_cameras_layout
{
	ShadowCamerasData ubo_shadowCameras;
};

// Outputs

out GeometryOutput
{
	vec2 texCoords;
	float depth;
} io_geometryOutput;


// Function definitions

void main()
{
	// For each shadow camera emit a corresponding triangle in the corresponding layer

	for (int i = 0; i < ubo_shadowCameras.count; ++i)
	{
		for (uint j = 0; j < 3; ++j)
		{
			gl_Layer = i;
			
			const vec4 position = ubo_shadowCameras.cameras[i].view * gl_in[j].gl_Position;

			io_geometryOutput.texCoords = io_vertexOutput[j].texCoords;
			io_geometryOutput.depth = 1.0 - (ubo_shadowCameras.cameras[i].far + position.z) / (ubo_shadowCameras.cameras[i].far - ubo_shadowCameras.cameras[i].near);
				
			gl_Position = ubo_shadowCameras.cameras[i].projection * position;

			EmitVertex();
		}

		EndPrimitive();
	}
}
