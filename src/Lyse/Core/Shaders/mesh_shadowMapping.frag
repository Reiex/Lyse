///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in vec2 io_texCoords;

// Uniforms

#ifdef COLOR_TEXTURE
	uniform sampler2D u_drawableColor;
#else
	uniform vec4 u_drawableColor;
#endif

// Function definitions

void main()
{
	#ifdef COLOR_TEXTURE
		vec4 color = texture(u_drawableColor, io_texCoords);
	#else
		vec4 color = u_drawableColor;
	#endif

	if (color.a != 1.0)
	{
		discard;
	}
}
