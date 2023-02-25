///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Lyse/Core/CoreDecl.hpp>

namespace lys
{
	constexpr CameraBase::CameraBase() : Transformable(),
		_uboCameraData(),
		_uboCamera(),
		_uboNeedsUpdate(true),
		_frontNeedsUpdate(true),
		_upNeedsUpdate(true),
		_rightNeedsUpdate(true),
		_viewNeedsUpdate(true),
		_invViewNeedsUpdate(true),
		_projectionNeedsUpdate(true),
		_invProjectionNeedsUpdate(true),
		_projectionViewNeedsUpdate(true),
		_invProjectionViewNeedsUpdate(true)
	{
	}

	inline void CameraBase::lookAt(const scp::f32vec3& position, float dutchAngle)
	{
		if (distance(position, getTranslation()) == 0.f)
		{
			return;
		}

		scp::Quat<float> rot = { 1.f, 0.f, 0.f, 0.f };
		setRotation(rot);

		scp::f32vec3 dir = normalize(position - getTranslation());

		scp::f32vec3 dirPlane = { dir.x, 0.f, dir.z };
		if (scp::length(dirPlane) != 0.f)
		{
			const float angle = std::atan2(-dirPlane.x, -dirPlane.z);

			rotate(getUpVector(), angle);
		}

		dirPlane = { 0.f, dir.y, dir.z };
		if (scp::length(dirPlane) != 0.f)
		{
			const float angle = std::numbers::pi / 2 - std::acos(dot(dirPlane, { 0.f, 1.f, 0.f }));

			rotate(getRightVector(), angle);
		}

		if (dutchAngle != 0.f)
		{
			rotate(getFrontVector(), dutchAngle);
		}
	}

	inline void CameraBase::updateAndBindUbo(uint32_t bindingIndex) const
	{
		getFrontVector();
		getUpVector();
		getRightVector();
		getViewMatrix();
		getInverseViewMatrix();
		getProjectionMatrix();
		getInverseProjectionMatrix();
		getProjectionViewMatrix();
		getInverseProjectionViewMatrix();

		if (_uboNeedsUpdate)
		{
			if (_uboCamera.has_value())
			{
				_uboCamera->update(&_uboCameraData, sizeof(UboCameraData));
			}
			else
			{
				_uboCamera.emplace(sizeof(UboCameraData), spl::BufferStorageFlags::DynamicStorage, &_uboCameraData);
			}

			_uboNeedsUpdate = false;
		}

		spl::Buffer::bind(*_uboCamera, spl::BufferTarget::Uniform, bindingIndex);
	}

	constexpr float CameraBase::getAspect() const
	{
		return _uboCameraData.aspect;
	}

	constexpr float CameraBase::getFieldOfView() const
	{
		return _uboCameraData.fov;
	}

	constexpr float CameraBase::getNearDistance() const
	{
		return _uboCameraData.near;
	}

	constexpr float CameraBase::getFarDistance() const
	{
		return _uboCameraData.far;
	}

	constexpr const scp::f32vec3& CameraBase::getFrontVector() const
	{
		_checkTransformHasChanged();

		if (_frontNeedsUpdate)
		{
			_uboCameraData.front.x = 0.f;
			_uboCameraData.front.y = 0.f;
			_uboCameraData.front.z = -1.f;

			applyRotationTo(_uboCameraData.front);

			_frontNeedsUpdate = false;
		}

		return _uboCameraData.front;
	}

	constexpr const scp::f32vec3& CameraBase::getUpVector() const
	{
		_checkTransformHasChanged();

		if (_upNeedsUpdate)
		{
			_uboCameraData.up.x = 0.f;
			_uboCameraData.up.y = 1.f;
			_uboCameraData.up.z = 0.f;

			applyRotationTo(_uboCameraData.up);

			_upNeedsUpdate = false;
		}

		return _uboCameraData.up;
	}

	constexpr const scp::f32vec3& CameraBase::getRightVector() const
	{
		_checkTransformHasChanged();

		if (_rightNeedsUpdate)
		{
			_uboCameraData.right.x = 1.f;
			_uboCameraData.right.y = 0.f;
			_uboCameraData.right.z = 0.f;

			applyRotationTo(_uboCameraData.right);

			_rightNeedsUpdate = false;
		}

		return _uboCameraData.right;
	}

	constexpr const scp::f32mat4x4& CameraBase::getViewMatrix() const
	{
		_checkTransformHasChanged();

		if (_viewNeedsUpdate)
		{
			_uboCameraData.view = getInverseTransformMatrix();
			_viewNeedsUpdate = false;
		}

		return _uboCameraData.view;
	}

	constexpr const scp::f32mat4x4& CameraBase::getInverseViewMatrix() const
	{
		_checkTransformHasChanged();

		if (_invViewNeedsUpdate)
		{
			_uboCameraData.invView = getTransformMatrix();
			_invViewNeedsUpdate = false;
		}

		return _uboCameraData.invView;
	}

	constexpr const scp::f32mat4x4& CameraBase::getProjectionMatrix() const
	{
		if (_projectionNeedsUpdate)
		{
			_updateProjectionMatrix();
			_projectionNeedsUpdate = false;
		}

		return _uboCameraData.projection;
	}

	constexpr const scp::f32mat4x4& CameraBase::getInverseProjectionMatrix() const
	{
		if (_invProjectionNeedsUpdate)
		{
			_updateInverseProjectionMatrix();
			_invProjectionNeedsUpdate = false;
		}

		return _uboCameraData.invProjection;
	}

	constexpr const scp::f32mat4x4& CameraBase::getProjectionViewMatrix() const
	{
		_checkTransformHasChanged();

		if (_projectionViewNeedsUpdate)
		{
			_uboCameraData.projectionView = getProjectionMatrix() * getViewMatrix();
			_projectionViewNeedsUpdate = false;
		}

		return _uboCameraData.projectionView;
	}

	constexpr const scp::f32mat4x4& CameraBase::getInverseProjectionViewMatrix() const
	{
		_checkTransformHasChanged();

		if (_invProjectionViewNeedsUpdate)
		{
			_uboCameraData.invProjectionView = getInverseViewMatrix() * getInverseProjectionMatrix();
			_invProjectionViewNeedsUpdate = false;
		}

		return _uboCameraData.invProjectionView;
	}

	constexpr const scp::f32mat4x4& CameraBase::getTransformMatrix() const
	{
		_checkTransformHasChanged();
		return Transformable::getTransformMatrix();
	}

	constexpr void CameraBase::_checkTransformHasChanged() const
	{
		if (!_transform.has_value())
		{
			Transformable::getTransformMatrix();
			_viewDependentNeedsUpdate();
		}
	}

	constexpr void CameraBase::_viewDependentNeedsUpdate() const
	{
		_uboNeedsUpdate = true;

		_frontNeedsUpdate = true;
		_upNeedsUpdate = true;
		_rightNeedsUpdate = true;
		_viewNeedsUpdate = true;
		_invViewNeedsUpdate = true;
		_projectionViewNeedsUpdate = true;
		_invProjectionViewNeedsUpdate = true;
	}

	constexpr void CameraBase::_projectionDependentNeedsUpdate() const
	{
		_uboNeedsUpdate = true;

		_projectionNeedsUpdate = true;
		_invProjectionNeedsUpdate = true;
		_projectionViewNeedsUpdate = true;
		_invProjectionViewNeedsUpdate = true;
	}
}
