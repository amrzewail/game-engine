#pragma once

#include "Quaternion.hpp"


Quaternion::Quaternion()
{
	Set(1, 0, 0, 0);
}

Quaternion::Quaternion(float w, float x, float y, float z)
{
	Set(w, x, y, z);
}

Quaternion::~Quaternion()
{}

Quaternion::Quaternion(const Quaternion& source)
{
	Set(source.w, source.x, source.y, source.z);
}


Quaternion Quaternion::Identity()
{
	return Quaternion(1, 0, 0, 0);
}


void Quaternion::Set(float w, float x, float y, float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
	Normalize();
}


Quaternion Quaternion::Euler(Vector euler)
{
	float x = euler.x * Math::Deg2Rad;
	float y = euler.y * Math::Deg2Rad;
	float z = euler.z * Math::Deg2Rad;

	float cx = Math::Cos(x * 0.5f);
	float sx = Math::Sin(x * 0.5f);
	float cy = Math::Cos(y * 0.5f);
	float sy = Math::Sin(y * 0.5f);
	float cz = Math::Cos(z * 0.5f);
	float sz = Math::Sin(z * 0.5f);

	Quaternion qZ(cz, 0, 0, sz); // Z rotation
	Quaternion qX(cx, sx, 0, 0); // X rotation
	Quaternion qY(cy, 0, sy, 0); // Y rotation

	// Apply Z → X → Y => q = qY * qX * qZ
	Quaternion q = qY * qX * qZ;
	return q.Normalized();
}

// roll (x), pitch (Y), yaw (z)
// Euler by ZXY
Quaternion Quaternion::Euler(float x, float y, float z)
{
	return Euler(Vector(x, y, z));
}

// roll (x), pitch (Y), yaw (z)
Vector Quaternion::EulerAngles() const
{
	Vector euler;
	float test = 2.0f * (w * y - z * x);

	if (Math::Abs(test) < 0.99999f)
	{
		euler.x = Math::Asin(Math::Clamp(test, -1.0f, 1.0f));
		euler.y = Math::Atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
		euler.z = Math::Atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
	}
	else
	{
		// Gimbal lock case
		euler.x = Math::CopySign(Math::PI / 2.0f, test);
		euler.y = Math::Atan2(-2.0f * (x * z - w * y), 1.0f - 2.0f * (x * x + y * y));
		euler.z = 0.0f;
	}

	euler = euler * Math::Rad2Deg;
	return NormalizeAngles(euler);
}

Vector Quaternion::NormalizeAngles(Vector angles)
{
	angles.x = NormalizeAngle(angles.x);
	angles.y = NormalizeAngle(angles.y);
	angles.z = NormalizeAngle(angles.z);
	return angles;
}
float Quaternion::NormalizeAngle(float angle)
{
	float modAngle = Math::Mod(angle, 360.0f);

	if (modAngle < 0.0f)
		return modAngle + 360.0f;
	else
		return modAngle;
}

std::string Quaternion::ToString()
{
	return "(w:" + std::to_string(w) + ", x:" + std::to_string(x) + ", y:" + std::to_string(y) + ", z:" + std::to_string(z) + ")";
}

void Quaternion::Normalize()
{
	float d = sqrt(w * w + x * x + y * y + z * z);
	if (!Math::Approximately(d, 0))
	{
		w /= d;
		x /= d;
		y /= d;
		z /= d;
	}
}

Quaternion Quaternion::Normalized() const
{
	float d = sqrt(w * w + x * x + y * y + z * z);
	return Quaternion(w / d, x / d, y / d, z / d);
}


Quaternion Quaternion::operator=(const Quaternion rhs)
{
	if (this != &rhs)
	{
		Set(rhs.w, rhs.x, rhs.y, rhs.z);
	}
	return *this;
}

Quaternion Quaternion::operator=(const Vector euler)
{
	*this = Quaternion::Euler(euler.x, euler.y, euler.z);
	return *this;
}

Vector Quaternion::operator*(const Vector rhs)
{
	//https://github.com/Unity-Technologies/UnityCsReference/blob/0a2eeb7/Runtime/Export/Math/Quaternion.cs#L96-L117

	float x = this->x * 2.0f;
	float y = this->y * 2.0f;
	float z = this->z * 2.0f;
	float xx = this->x * x;
	float yy = this->y * y;
	float zz = this->z * z;
	float xy = this->x * y;
	float xz = this->x * z;
	float yz = this->y * z;
	float wx = this->w * x;
	float wy = this->w * y;
	float wz = this->w * z;

	Vector res;
	res.x = (1.0f - (yy + zz)) * rhs.x + (xy - wz) * rhs.y + (xz + wy) * rhs.z;
	res.y = (xy + wz) * rhs.x + (1.0f - (xx + zz)) * rhs.y + (yz - wx) * rhs.z;
	res.z = (xz - wy) * rhs.x + (yz + wx) * rhs.y + (1.0f - (xx + yy)) * rhs.z;

	return res;
}

Quaternion Quaternion::operator*(const Quaternion rhs)
{
	return Quaternion(
		w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
		w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
		w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
		w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
	);
}