#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

struct Quaternion
{
private:
	fquat* _quaternion;

public:

	static Quaternion* Identity()
	{
		return new Quaternion(1, 0, 0, 0);
	}

	// roll (x), pitch (Y), yaw (z)
	static Quaternion* Euler(float x, float y, float z)
	{
		x = glm::radians(x);
		y = glm::radians(y);
		z = glm::radians(z);
		/*
		float cosTheta1 = cos(x * 0.5);
		float sinTheta1 = sin(x * 0.5);
		float cosTheta2 = cos(y * 0.5);
		float sinTheta2 = sin(y * 0.5);
		float cosTheta3 = cos(z * 0.5);
		float sinTheta3 = sin(z * 0.5);

		float w = -sinTheta1 * sinTheta2 * sinTheta3 + cosTheta1 * cosTheta2 * cosTheta3;
		y = -sinTheta1 * cosTheta2 * sinTheta3 + cosTheta1 * sinTheta2 * cosTheta3;
		z = sinTheta1 * sinTheta2 * cosTheta3 + cosTheta1 * cosTheta2 * sinTheta3;
		x = sinTheta1 * cosTheta2 * cosTheta3 + cosTheta1 * sinTheta2 * sinTheta3;*/

		glm::fquat q = glm::fquat(glm::vec3(z, x, y));

		Quaternion* quaternion = new Quaternion(q.w, q.y, q.z, q.x);

		quaternion->Normalize();

		return quaternion;
	}

	Quaternion(float w, float x, float y, float z)
	{
		_quaternion = new fquat(w, x, y, z);
	}

	~Quaternion()
	{
		delete _quaternion;
	}

	float X() { return _quaternion->x; }
	float Y() { return _quaternion->y; }
	float Z() { return _quaternion->z; }
	float W() { return _quaternion->w; }

	void Set(float w, float x, float y, float z)
	{
		_quaternion->w = w;
		_quaternion->x = x;
		_quaternion->y = y;
		_quaternion->z = z;
	}

	Vector* EulerAngles()
	{
		glm::vec3 euler = glm::eulerAngles(fquat(W(), Z(), X(), Y()));
		euler.x = glm::degrees(euler.x);
		euler.y = glm::degrees(euler.y);
		euler.z = glm::degrees(euler.z);

		euler.x = euler.x < 0 ? euler.x + 360 : euler.x;
		euler.y = euler.y < 0 ? euler.y + 360 : euler.y;
		euler.z = euler.z < 0 ? euler.z + 360 : euler.z;
		return new Vector(euler.y, euler.z, euler.x);
	}

	std::string ToString()
	{
		return '(' + std::to_string(W()) + ", " + std::to_string(X()) + ", " + std::to_string(Y()) + ", " + std::to_string(Z()) + ')';
	}

	void Normalize()
	{
		float d = sqrt(pow(W(), 2) + pow(X(), 2) + pow(Y(), 2) + pow(Z(), 2));
		Set(W() / d, X() / d, Y() / d, Z() / d);
	}

	Quaternion* Normalized()
	{
		float d = sqrt(pow(W(), 2) + pow(X(), 2) + pow(Y(), 2) + pow(Z(), 2));
		return new Quaternion(W() / d, X() / d, Y() / d, Z() / d);
	}

	Quaternion* operator *(Quaternion& rhs) const
	{
		fquat rhsQ = fquat(rhs.W(), rhs.X(), rhs.Y(), rhs.Z());
		rhsQ = (* _quaternion) * rhsQ;
		return new Quaternion(rhsQ.w, rhsQ.x, rhsQ.y, rhsQ.z);
	}

};