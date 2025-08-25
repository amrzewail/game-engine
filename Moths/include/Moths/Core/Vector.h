#pragma once

#include <string>

#include <Moths/API.h>
#include <Moths/Core/Math.h>

namespace MothsEngine
{
	struct MOTHS_API Vector
	{
	private:

	public:
		float x;
		float y;
		float z;


		static Vector Zero()
		{
			return Vector(0, 0, 0);
		}

		static Vector One()
		{
			return Vector(1, 1, 1);
		}

		static Vector Up()
		{
			return Vector(0, 1, 0);
		}

		static Vector Down()
		{
			return Vector(0, -1, 0);
		}

		static Vector Right()
		{
			return Vector(1, 0, 0);
		}

		static Vector Left()
		{
			return Vector(-1, 0, 0);
		}

		static Vector Forward()
		{
			return Vector(0, 0, 1);
		}

		static Vector Back()
		{
			return Vector(0, 0, -1);
		}

		static float Dot(const Vector& v1, const Vector& v2)
		{
			return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		}

		static Vector Cross(const Vector& v1, const Vector& v2)
		{
			Vector v;
			v.x = v1.y * v2.z - v1.z * v2.y;
			v.y = v1.z * v2.x - v1.x * v2.z;
			v.z = v1.x * v2.y - v1.y * v2.x;
			return v;
		}

		static float Angle(const Vector& v1, const Vector& v2)
		{
			float ab = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
			return acosf(ab / (v1.Magnitude() * v2.Magnitude()));
		}

		float Magnitude() const
		{
			return Math::Sqrt(MagnitudeSquared());
		}

		float MagnitudeSquared() const
		{
			return x * x + y * y + z * z;
		}

		void Normalize()
		{
			float length = Magnitude();
			if (length <= Math::Epsilon) return;
			x /= length;
			y /= length;
			z /= length;
		}


		Vector()
		{
			x = y = z = 0;
		}

		Vector(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector(const Vector& source)
		{
			this->x = source.x;
			this->y = source.y;
			this->z = source.z;
		}


		std::string ToString()
		{
			return '(' + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ')';
		}


		Vector operator =(const Vector rhs)
		{
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}


		Vector operator+= (const Vector rhs)
		{
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}


		Vector operator-= (const Vector rhs)
		{
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		friend Vector operator +(Vector lhs, Vector rhs)
		{
			lhs += rhs;
			return lhs;
		}

		friend Vector operator -(Vector lhs, Vector rhs)
		{
			lhs -= rhs;
			return lhs;
		}


		Vector operator* (const float rhs)
		{
			this->x *= rhs;
			this->y *= rhs;
			this->z *= rhs;
			return *this;
		}

		const Vector operator-()
		{
			return Vector(-x, -y, -z);
		}
	};
}