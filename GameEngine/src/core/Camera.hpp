#pragma once
#include "Transform.hpp"


class Camera
{
private:
	Matrix4x4* _projection;


public:
	Transform* transform;

	float fieldOfView = 60;
	float aspect = 800.0 / 600.0;
	float nearClipPlane = 0.3f;
	float farClipPlane = 100000.0f;

	Camera()
	{
		transform = new Transform();
	}

	~Camera()
	{
		delete transform;
		delete _projection;
	}


	Matrix4x4 ProjectionMatrix()
	{
		Matrix4x4 transformation = transform->LocalToWorldMatrix();
		Matrix4x4 projection = *Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);
		Matrix4x4 view = *Matrix4x4::Inverse(transformation);
		_projection = projection * view;
		return *_projection;
	}
};