///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in GeometryOutput
{
	vec2 texCoords;
	float depth;
} io_geometryOutput;

// Uniforms

#ifdef COLOR_TEXTURE
	uniform sampler2D u_drawableColor;
#else
	uniform vec4 u_drawableColor;
#endif

// Function definitions

void main()
{
	// If the fragment has transparency, it doesn't cast any shadow (convenient approximation) thus it is discarded

	#ifdef COLOR_TEXTURE
		vec4 color = texture(u_drawableColor, io_geometryOutput.texCoords);
	#else
		vec4 color = u_drawableColor;
	#endif

	if (color.a != 1.0)
	{
		discard;
	}

	gl_FragDepth = io_geometryOutput.depth;
}
