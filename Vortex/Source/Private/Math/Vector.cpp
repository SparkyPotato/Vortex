#include <VXpch.h>
#include <Math/Vector.h>
#include <Math/Matrix.h>

namespace Vortex
{
namespace Math
{
	Vector::Vector()
	{
		x = y = z = 0.f;
		w = 1.f;
	}

	Vector::Vector(float all)
	{
		x = y = z = w = all;
	}

	Vector::Vector(float xv, float yv, float zv)
	{
		x = xv;
		y = yv;
		z = zv;
		w = 1.f;
	}

	Vector::Vector(float xv, float yv, float zv, float wv)
	{
		x = xv;
		y = yv;
		z = zv;
		w = wv;
	}

	Vector::Vector(const Vector& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	Vector::Vector(float values[3])
	{
		x = values[0];
		y = values[1];
		z = values[2];
		w = 1.f;
	}

	Vector Vector::operator+(const Vector& other) const
	{
		return Vector(x + other.x, y + other.y, z + other.z);
	}

	Vector Vector::operator+(float all) const
	{
		return Vector(x + all, y + all, z + all);
	}

	Vector& Vector::operator+=(const Vector& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector& Vector::operator+=(float all)
	{
		x += all;
		y += all;
		z += all;

		return *this;
	}

	Vector Vector::operator-() const
	{
		return Vector(-x, -y, -z);
	}

	Vector Vector::operator-(const Vector& other) const
	{
		return Vector(x - other.x, y - other.y, z - other.z);
	}

	Vector Vector::operator-(float all) const
	{
		return Vector(x - all, y - all, z - all);
	}

	Vector& Vector::operator-=(const Vector& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	Vector& Vector::operator-=(float all)
	{
		x -= all;
		y -= all;
		z -= all;

		return *this;
	}

	Vector Vector::operator*(float scalar) const
	{
		return Vector(x * scalar, y * scalar, z * scalar);
	}

	Vector& Vector::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector Vector::operator*(const Matrix& transform) const
	{
		Vector vec;

		vec.x = x * transform.columns[0].x + y * transform.columns[0].y + z * transform.columns[0].z + w * transform.columns[0].w;
		vec.y = x * transform.columns[1].x + y * transform.columns[1].y + z * transform.columns[1].z + w * transform.columns[1].w;
		vec.z = x * transform.columns[2].x + y * transform.columns[2].y + z * transform.columns[2].z + w * transform.columns[2].w;
		vec.w = x * transform.columns[3].x + y * transform.columns[3].y + z * transform.columns[3].z + w * transform.columns[3].w;

		return vec;
	}

	Vector& Vector::operator*=(const Matrix& transform)
	{
		*this = *this * transform;

		return *this;
	}

	Vector Vector::operator/(float scalar) const
	{
		return Vector(x / scalar, y / scalar, z / scalar);
	}

	Vector& Vector::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	float Vector::GetLength()
	{
		float square = x * x + y * y + z * z;
		return std::sqrt(square);
	}

	Vector& Vector::Normalize()
	{
		*this /= GetLength();
		return *this;
	}

	Vector Vector::GetNormalized()
	{
		return *this / GetLength();
	}

	float Vector::Dot3D(const Vector& first, const Vector& second)
	{
		float dot = 0.f;

		dot += first.x * second.x;
		dot += first.y * second.y;
		dot += first.z * second.z;

		return dot;
	}

	Vector Vector::Cross3D(const Vector& first, const Vector& second)
	{
		Vector vec;

		vec.x = (first.y * second.z) - (first.z * second.y);
		vec.y = (first.z * second.x) - (first.x * second.z);
		vec.z = (first.x * second.y) - (first.y * second.x);

		return vec;
	}

	Vector Vector::Hadamard3D(const Vector& first, const Vector& second)
	{
		Vector vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;

		return vec;
	}

	float Vector::Dot4D(const Vector& first, const Vector& second)
	{
		float dot = 0.f;

		dot += first.x * second.x;
		dot += first.y * second.y;
		dot += first.z * second.z;
		dot += first.w * second.w;

		return dot;
	}

	Vector Vector::Hadamard4D(const Vector& first, const Vector& second)
	{
		Vector vec;

		vec.x = first.x * second.x;
		vec.y = first.y * second.y;
		vec.z = first.z * second.z;
		vec.w = first.w * second.w;

		return vec;
	}
}
}
