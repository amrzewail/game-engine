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
	float nearClipPlane = 0.01f;
	float farClipPlane = 1000.0f;

	Camera()
	{
		transform = new Transform();
		_projection = new Matrix4x4();
	}

	~Camera()
	{
		delete transform;
		delete _projection;
	}

	void CalculateProjectionMatrix()
	{
		Matrix4x4 transformation = transform->LocalToWorldMatrix();
		Matrix4x4 view = Matrix4x4::Inverse(transformation);
		Matrix4x4 projection = Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);

		*_projection = projection * view;
	}


	Matrix4x4& ProjectionMatrix() const
	{
		return *_projection;
	}
};