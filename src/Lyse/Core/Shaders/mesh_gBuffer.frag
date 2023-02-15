///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


in VertexOutput
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} io_vertexOutput;


#ifdef COLOR_MAP
    uniform sampler2D u_color;
#else
    uniform vec3 u_color;
#endif

#ifdef MATERIAL_MAP
    uniform sampler2D u_material;
#else
    uniform vec3 u_material;
#endif

#ifdef NORMAL_MAP
    uniform sampler2D u_normalMap;
#endif


layout (location = 0) out vec3 fo_color;
layout (location = 1) out vec3 fo_material;
layout (location = 2) out vec3 fo_position;
layout (location = 3) out vec3 fo_normal;
layout (location = 4) out vec3 fo_tangent;


void main()
{
    #ifdef COLOR_MAP
        fo_color = texture(u_color, io_vertexOutput.texCoords).rgb;
    #else
        fo_color = u_color;
    #endif

    #ifdef MATERIAL_MAP
        fo_material = texture(u_material, io_vertexOutput.texCoords).rgb;
    #else
        fo_material = u_material;
    #endif

    fo_position = io_vertexOutput.position;
    fo_normal = normalize(io_vertexOutput.normal);
    fo_tangent = normalize(io_vertexOutput.tangent);

    #ifdef NORMAL_MAP
        vec3 normal = normalize(texture(u_normalMap, io_vertexOutput.texCoords).rgb);
        fo_normal = normalize(fo_tangent * normal.x + cross(fo_normal, fo_tangent) * normal.y + fo_normal * normal.z);
    #endif
} 
