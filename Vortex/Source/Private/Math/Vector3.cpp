#include <VXpch.h>
#include <Math/Vector.h>

namespace Vortex
{
namespace Math
{
	Vector3::Vector3()
	{
		x = y = z = 0.f;
	}

	Vector3::Vector3(float all)
	{
		x = y = z = all;
	}

	Vector3::Vector3(float xv, float yv, float zv)
	{
		x = xv;
		y = yv;
		z = zv;
	}

	Vector3::Vector3(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vector3 Vector3::operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 Vector3::operator+(float all)
	{
		return Vector3(x + all, y + all, z + all);
	}

	Vector3& Vector3::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector3& Vector3::operator+=(float all)
	{
		x += all;
		y += all;
		z += all;

		return *this;
	}

	Vector3 Vector3::operator-()
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 Vector3::operator-(float all)
	{
		return Vector3(x - all, y - all, z - all);
	}

	Vector3& Vector3::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector3& Vector3::operator-=(float all)
	{
		x -= all;
		y -= all;
		z -= all;

		return *this;
	}

	Vector3 Vector3::operator*(float scalar)
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3& Vector3::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector3 Vector3::operator/(float scalar)
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	Vector3& Vector3::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	Vector3 Vector3::Dot(const Vector3& first, const Vector3& second)
	{
		Vector3 vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;

		return vec;
	}

	Vector3 Vector3::Cross(const Vector3& first, const Vector3& second)
	{
		Vector3 vec;

		vec.x = (first.y * second.z) - (first.z * second.y);
		vec.y = (first.z * second.x) - (first.x * second.z);
		vec.z = (first.x * second.y) - (first.y * second.x);

		return vec;
	}

	Vector3 Vector3::Hadamard(const Vector3& first, const Vector3& second)
	{
		Vector3 vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;

		return vec;
	}
}
}
