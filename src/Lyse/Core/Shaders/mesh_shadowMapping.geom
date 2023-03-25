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
	vec3 normal;
	float depth;
} io_geometryOutput;


// Function definitions

void main()
{
	// For each shadow camera emit a corresponding triangle in the corresponding layer

	for (uint i = 0; i < ubo_shadowCameras.count; ++i)
	{
		const vec4 positions[3] = {
			ubo_shadowCameras.cameras[i].view * gl_in[0].gl_Position,
			ubo_shadowCameras.cameras[i].view * gl_in[1].gl_Position,
			ubo_shadowCameras.cameras[i].view * gl_in[2].gl_Position
		};

		const vec3 normal = normalize(cross(positions[1].xyz - positions[0].xyz, positions[2].xyz - positions[0].xyz));

		for (uint j = 0; j < 3; ++j)
		{
			gl_Layer = int(i);
				
			io_geometryOutput.texCoords = io_vertexOutput[j].texCoords;
			io_geometryOutput.normal = normal;
			io_geometryOutput.depth = 1.0 - (ubo_shadowCameras.cameras[i].far + positions[j].z) / (ubo_shadowCameras.cameras[i].far - ubo_shadowCameras.cameras[i].near);
				
			gl_Position = ubo_shadowCameras.cameras[i].projection * positions[j];

			EmitVertex();
		}

		EndPrimitive();
	}
}
