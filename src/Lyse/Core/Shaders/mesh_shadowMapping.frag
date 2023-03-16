///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in vec2 io_texCoords;

// Uniforms

#ifdef COLOR_MAP
	uniform sampler2D u_color;
#else
	uniform vec4 u_color;
#endif

// Function definitions

void main()
{
	#ifdef COLOR_MAP
		vec4 color = texture(u_color, io_texCoords);
	#else
		vec4 color = u_color;
	#endif

	if (color.a < 0.2)
	{
		discard;
	}
}
