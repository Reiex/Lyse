#######################################################################################################################
##! \file
##! \author Pélégrin Marius
##! \copyright The MIT License (MIT)
##! \date 2022-2023
#######################################################################################################################

# Global project definitions

cmake_minimum_required(VERSION 3.21)

project(Lyse)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add external dependencies

if(NOT TARGET scipp)
    set(SCIPP_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SciPP)
endif()

if(NOT TARGET ruc)
    set(RUC_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Ruc)
endif()

if(NOT TARGET diskon)
    set(DISKON_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/Diskon)
endif()

if(NOT TARGET dejavu)
    set(DEJAVU_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/DejaVu)
endif()

if(NOT TARGET splaylibrary)
    set(SPLAYLIBRARY_ADD_EXAMPLES OFF)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SplayLibrary)
endif()

# Lyse

add_library(
    lyse SHARED
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Lyse.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/LyseDecl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/LyseTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Core.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/CoreDecl.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/CoreTypes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/CameraBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/CameraOrthographic.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/CameraPerspective.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Drawable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/DrawableGroup.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/LightBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/LightPoint.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/LightSpot.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/LightSun.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Material.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/MaterialBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Mesh.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Scene.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/ShaderSet.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Transformable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/Vertex.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/CameraBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/CameraOrthographic.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/CameraPerspective.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/LightBase.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/LightPoint.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/LightSpot.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/LightSun.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/Mesh.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/Transformable.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/Lyse/Core/templates/Vertex.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Drawable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Material.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Scene.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/ShaderSet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Core/Shaders/shaders.cpp
)

add_dependencies(
    lyse
    scipp
    ruc
    diskon
    dejavu
    splaylibrary
)

target_include_directories(
    lyse
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/SciPP/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Ruc/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Diskon/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/DejaVu/include
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/SplayLibrary/include
)

target_compile_definitions(
    lyse
    PUBLIC LYSE_EXPORTS
)

target_link_libraries(
    lyse
    diskon
    splaylibrary
)

# Lyse examples

option(LYSE_ADD_EXAMPLES "Add target lyse-examples" ON)

if(LYSE_ADD_EXAMPLES)

    add_executable(
        lyse-examples
        ${CMAKE_CURRENT_LIST_DIR}/examples/main.cpp
    )

    add_dependencies(
        lyse-examples
        lyse
    )

    target_include_directories(
        lyse-examples
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/SciPP/include
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Ruc/include
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/Diskon/include
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/DejaVu/include
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/external/SplayLibrary/include
    )

    target_link_libraries(
        lyse-examples
        lyse
    )

endif()
