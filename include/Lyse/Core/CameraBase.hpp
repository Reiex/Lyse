///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreTypes.hpp>

namespace lys
{
	enum class CameraType
	{
		Orthographic,
		Perspective
	};

	class CameraBase : public Transformable
	{
		public:

			// TODO: I have a dream ! That maths functions have a constexpr implementation !
			inline void lookAt(const scp::f32vec3& position, float dutchAngle = 0.f);


			inline void updateAndBindUbo(uint32_t bindingIndex) const;


			virtual constexpr CameraType getType() const = 0;

			constexpr float getAspect() const;
			constexpr float getFieldOfView() const;
			constexpr float getNearDistance() const;
			constexpr float getFarDistance() const;

			constexpr const scp::f32vec3& getFrontVector() const;
			constexpr const scp::f32vec3& getUpVector() const;
			constexpr const scp::f32vec3& getRightVector() const;

			constexpr const scp::f32mat4x4& getViewMatrix() const;
			constexpr const scp::f32mat4x4& getInverseViewMatrix() const;
			constexpr const scp::f32mat4x4& getProjectionMatrix() const;
			constexpr const scp::f32mat4x4& getInverseProjectionMatrix() const;
			constexpr const scp::f32mat4x4& getProjectionViewMatrix() const;
			constexpr const scp::f32mat4x4& getInverseProjectionViewMatrix() const;

			virtual constexpr const scp::f32mat4x4& getTransformMatrix() const override final;

		protected:

			constexpr CameraBase();
			constexpr CameraBase(const CameraBase& camera) = default;
			constexpr CameraBase(CameraBase&& camera) = default;

			constexpr CameraBase& operator=(const CameraBase& camera) = default;
			constexpr CameraBase& operator=(CameraBase&& camera) = default;

			constexpr void _checkTransformHasChanged() const;
			constexpr void _viewDependentNeedsUpdate() const;
			constexpr void _projectionDependentNeedsUpdate() const;
			virtual constexpr void _updateProjectionMatrix() const = 0;
			virtual constexpr void _updateInverseProjectionMatrix() const = 0;

			virtual constexpr ~CameraBase() override = default;

			#pragma pack(push, 1)
			struct alignas(16) UboCameraData
			{
				alignas(4) float aspect;
				alignas(4) float fov;
				alignas(4) float near;
				alignas(4) float far;

				alignas(16) scp::f32vec3 front;
				alignas(16) scp::f32vec3 up;
				alignas(16) scp::f32vec3 right;

				alignas(16) scp::f32mat4x4 view;
				alignas(16) scp::f32mat4x4 invView;
				alignas(16) scp::f32mat4x4 projection;
				alignas(16) scp::f32mat4x4 invProjection;
				alignas(16) scp::f32mat4x4 projectionView;
				alignas(16) scp::f32mat4x4 invProjectionView;
			};
			#pragma pack(pop)

			mutable UboCameraData _uboCameraData;
			mutable std::optional<spl::Buffer> _uboCamera;
			mutable bool _uboNeedsUpdate;

			mutable bool _frontNeedsUpdate;
			mutable bool _upNeedsUpdate;
			mutable bool _rightNeedsUpdate;
			mutable bool _viewNeedsUpdate;
			mutable bool _invViewNeedsUpdate;
			mutable bool _projectionNeedsUpdate;
			mutable bool _invProjectionNeedsUpdate;
			mutable bool _projectionViewNeedsUpdate;
			mutable bool _invProjectionViewNeedsUpdate;
	};
}
