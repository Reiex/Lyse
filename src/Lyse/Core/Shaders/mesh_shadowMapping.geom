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

in vec2 io_vertTexCoords[3];

// Uniform blocks

layout (std140, row_major, binding = 3) uniform ubo_shadow_cameras_layout
{
	ShadowCamerasData ubo_shadowCameras;
};

// Outputs

out vec2 io_texCoords;

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

		const float a = -2.0 / (ubo_shadowCameras.cameras[i].far - ubo_shadowCameras.cameras[i].near);
		const float b = 0.5 * (ubo_shadowCameras.cameras[i].far + ubo_shadowCameras.cameras[i].near) * a + 1e-4 / pow(normal.z, 2.0);

		for (uint j = 0; j < 3; ++j)
		{
			gl_Layer = int(i);
				
			io_texCoords = io_vertTexCoords[j];
				
			gl_Position = ubo_shadowCameras.cameras[i].projection * positions[j];
			gl_Position.z = gl_Position.w * (a * positions[j].z + b);

			EmitVertex();
		}

		EndPrimitive();
	}
}
