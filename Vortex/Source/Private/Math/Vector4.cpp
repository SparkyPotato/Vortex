#include <VXpch.h>
#include <Math/Vector.h>

namespace Vortex
{
namespace Math
{
	Vector4::Vector4()
	{
		x = y = z = 0.f;
		w = 1.f;
	}

	Vector4::Vector4(float all)
	{
		x = y = z = w = all;
	}

	Vector4::Vector4(float xv, float yv, float zv)
	{
		x = xv;
		y = yv;
		z = zv;
		w = 1.f;
	}

	Vector4::Vector4(float xv, float yv, float zv, float wv)
	{
		x = xv;
		y = yv;
		z = zv;
		w = wv;
	}

	Vector4::Vector4(const Vector3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = 1.f;
	}

	Vector4::Vector4(const Vector4& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	Vector4 Vector4::operator+(const Vector4& other)
	{
		return Vector4(x + other.x, y + other.y, z + other.z);
	}

	Vector4 Vector4::operator+(float all)
	{
		return Vector4(x + all, y + all, z + all);
	}

	Vector4& Vector4::operator+=(const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector4& Vector4::operator+=(float all)
	{
		x += all;
		y += all;
		z += all;

		return *this;
	}

	Vector4 Vector4::operator-()
	{
		return Vector4(-x, -y, -z);
	}

	Vector4 Vector4::operator-(const Vector4& other)
	{
		return Vector4(x - other.x, y - other.y, z - other.z);
	}

	Vector4 Vector4::operator-(float all)
	{
		return Vector4(x - all, y - all, z - all);
	}

	Vector4& Vector4::operator-=(const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector4& Vector4::operator-=(float all)
	{
		x -= all;
		y -= all;
		z -= all;

		return *this;
	}

	Vector4 Vector4::operator*(float scalar)
	{
		return Vector4(x * scalar, y * scalar, z * scalar);
	}

	Vector4& Vector4::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector4 Vector4::operator/(float scalar)
	{
		return Vector4(x / scalar, y / scalar, z / scalar);
	}

	Vector4& Vector4::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	Vector4 Vector4::Dot(const Vector4& first, const Vector4& second)
	{
		Vector4 vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;

		return vec;
	}

	Vector4 Vector4::Cross(const Vector4& first, const Vector4& second)
	{
		Vector4 vec;

		vec.x = (first.y * second.z) - (first.z * second.y);
		vec.y = (first.z * second.x) - (first.x * second.z);
		vec.z = (first.x * second.y) - (first.y * second.x);

		return vec;
	}

	Vector4 Vector4::Hadamard(const Vector4& first, const Vector4& second)
	{
		Vector4 vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;

		return vec;
	}
}
}
