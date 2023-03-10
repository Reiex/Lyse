#version 460 core

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs

in vec2 io_texCoords;

// Uniforms

uniform sampler2D u_scene;

// Outputs

layout (location = 0) out vec4 fo_output;

// Function definitions

void main()
{
    fo_output = texture(u_scene, io_texCoords);
}
