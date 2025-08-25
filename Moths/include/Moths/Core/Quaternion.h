#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>

#include <Moths/API.h>
#include <Moths/Core/Vector.h>

namespace MothsEngine
{
	struct MOTHS_API Quaternion
	{
	private:
		static Vector NormalizeAngles(Vector angles);
		static float NormalizeAngle(float angle);

	public:
		float w;
		float x;
		float y;
		float z;

		static Quaternion Identity();

		static Quaternion Euler(Vector euler);
		static Quaternion Euler(float x, float y, float z);


		Quaternion();

		Quaternion(float w, float x, float y, float z);

		~Quaternion();

		Quaternion(const Quaternion& source);

		void Set(float w, float x, float y, float z);

		// roll (x), pitch (Y), yaw (z)
		Vector EulerAngles() const;

		std::string ToString();

		void Normalize();

		Quaternion Normalized() const;

		Quaternion operator=(const Quaternion rhs);
		Quaternion operator=(const Vector euler);
		Vector operator*(const Vector rhs);
		Quaternion operator*(const Quaternion rhs);

		//friend Quaternion operator *(Quaternion lhs, Quaternion& rhs)
		//{
		//	//Quaternion quat = Quaternion::FromFQuat(Quaternion::ToFQuat(lhs) * Quaternion::ToFQuat(rhs));
		//	//lhs.Set(quat.w, quat.x, quat.y, quat.z));
		//	return lhs;
		//}

	};
}