///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Custom types 

struct CameraData
{
	float aspect;
	float fov;
	float near;
	float far;

	vec3 up;
	vec3 front;
	vec3 left;

	mat4 view;
	mat4 invView;
	mat4 projection;
	mat4 invProjection;
	mat4 projectionView;
	mat4 invProjectionView;
};

struct DrawableData
{
	mat4 model;
	mat4 invModel;
	mat4 viewModel;
	mat4 invViewModel;
	mat4 projectionViewModel;
	mat4 invProjectionViewModel;
};

// Constants

const float c_epsilon = 1e-5;
const float c_halfPi = 1.57079632679;
const float c_pi = 3.14159265359;
const float c_2pi = 6.28318530718;
const float c_invHalfPi = 0.63661977236;
const float c_invPi = 0.31830988618;
const float c_inv2pi = 0.15915494309;
