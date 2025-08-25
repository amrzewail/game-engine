#pragma once

#include <vector>

#include <Moths/API.h>
#include <Moths/Core/Transform.h>
#include <Moths/Core/Screen.h>

namespace MothsEngine
{
	class MOTHS_API Camera
	{
	private:
		Matrix4x4* _projection;
		static std::vector<Camera*> _cameras;

	public:
		Transform* transform;

		float fieldOfView = 60;
		float aspect = 800.0 / 600.0;
		float nearClipPlane = 0.01f;
		float farClipPlane = 1000.0f;

		Camera()
		{
			aspect = float(Screen::Width()) / Screen::Height();
			transform = new Transform();
			_projection = new Matrix4x4();

			_cameras.push_back(this);
		}

		~Camera()
		{
			_cameras.erase(std::remove(_cameras.begin(), _cameras.end(), this), _cameras.end());

			delete transform;
			delete _projection;
		}

		inline void CalculateProjectionMatrix()
		{
			Matrix4x4 transformation = transform->LocalToWorldMatrix();
			Matrix4x4 view = Matrix4x4::Inverse(transformation);
			Matrix4x4 projection = Matrix4x4::Perspective(fieldOfView, aspect, nearClipPlane, farClipPlane);

			*_projection = projection * view;
		}


		inline Matrix4x4& ProjectionMatrix() const
		{
			return *_projection;
		}

		inline static Camera* CurrentCamera()
		{
			if (_cameras.size() > 0) return _cameras[_cameras.size() - 1];
			return nullptr;
		}
	};
}