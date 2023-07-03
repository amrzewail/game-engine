#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

struct Quaternion
{
private:
	fquat* _quaternion;
	Vector* _eulerAngles;

public:

	static Quaternion* Identity()
	{
		return new Quaternion(1, 0, 0, 0);
	}

	// roll (x), pitch (Y), yaw (z)
	static Quaternion Euler(float x, float y, float z)
	{
		x = glm::radians(x);
		y = glm::radians(y);
		z = glm::radians(z);
		glm::fquat q = glm::fquat(glm::vec3(z, x, y));
		Quaternion quaternion = Quaternion(q.w, q.y, q.z, q.x);
		quaternion.Normalize();
		return quaternion;
	}

	Quaternion()
	{
		_quaternion = new fquat(1, 0, 0, 0);
		EulerAngles();

	}

	Quaternion(float w, float x, float y, float z)
	{
		_quaternion = new fquat(w, x, y, z);
		EulerAngles();
	}

	~Quaternion()
	{
		delete _quaternion;
		delete _eulerAngles;
	}

	Quaternion(const Quaternion& source)
	{
		_quaternion = new fquat(source.W(), source.X(), source.Y(), source.Z());
		EulerAngles();
	}


	Quaternion& operator=(const Quaternion& rhs)
	{
		if (this != &rhs)
		{
			delete _quaternion;
			delete _eulerAngles;
			_quaternion = new fquat(rhs.W(), rhs.X(), rhs.Y(), rhs.Z());
			EulerAngles();
		}
		return *this;
	}

	Quaternion& operator=(const Vector& euler)
	{
		delete _eulerAngles;
		float x = glm::radians(euler.x);
		float y = glm::radians(euler.y);
		float z = glm::radians(euler.z);
		fquat quat = glm::fquat(glm::vec3(z, x, y));
		_quaternion->w = quat.w;
		_quaternion->x = quat.y;
		_quaternion->y = quat.z;
		_quaternion->z = quat.x;
		Normalize();
		EulerAngles();
		return *this;
	}

	float X() const { return _quaternion->x; }
	float Y() const { return _quaternion->y; }
	float Z() const { return _quaternion->z; }
	float W() const { return _quaternion->w; }

	void Set(float w, float x, float y, float z)
	{
		_quaternion->w = w;
		_quaternion->x = x;
		_quaternion->y = y;
		_quaternion->z = z;
	}

	Vector& EulerAngles()
	{
		glm::vec3 euler = glm::eulerAngles(fquat(W(), Z(), X(), Y()));
		euler.x = glm::degrees(euler.x);
		euler.y = glm::degrees(euler.y);
		euler.z = glm::degrees(euler.z);

		euler.x = euler.x < 0 ? euler.x + 360 : euler.x;
		euler.y = euler.y < 0 ? euler.y + 360 : euler.y;
		euler.z = euler.z < 0 ? euler.z + 360 : euler.z;
		_eulerAngles = new Vector(euler.y, euler.z, euler.x);
		return *_eulerAngles;
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

	friend Quaternion operator *(Quaternion lhs, Quaternion& rhs)
	{
		fquat rhsQ = fquat(rhs.W(), rhs.X(), rhs.Y(), rhs.Z());
		rhsQ = *lhs._quaternion * rhsQ;
		lhs.Set(rhsQ.w, rhsQ.x, rhsQ.y, rhsQ.z);
	}

	friend Vector operator *(const Quaternion lhs, const Vector& rhs)
	{
		vec3 result =  fquat(lhs.W(), -lhs.X(), -lhs.Y(), lhs.Z()) * vec3(rhs.x, rhs.y, rhs.z);
		Vector vector = Vector(result.x, result.y, result.z);
		return vector;
	}

};