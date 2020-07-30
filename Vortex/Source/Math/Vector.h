#pragma once

namespace Vortex
{
namespace Math
{
	struct Vector3
	{
	public:
		Vector3();
		Vector3(float all);
		Vector3(float x, float y, float z);
		Vector3(const Vector3& other);

		Vector3 operator+(const Vector3& other);
		Vector3 operator+(float all);
		Vector3& operator+=(const Vector3& other);
		Vector3& operator+=(float all);

		Vector3 operator-();
		Vector3 operator-(const Vector3& other);
		Vector3 operator-(float all);
		Vector3& operator-=(const Vector3& other);
		Vector3& operator-=(float all);

		Vector3 operator*(float scalar);
		Vector3& operator*=(float scalar);

		Vector3 operator/(float scalar);
		Vector3& operator/=(float scalar);

		static Vector3 Dot(const Vector3& first, const Vector3& second);
		static Vector3 Cross(const Vector3& first, const Vector3& second);
		static Vector3 Hadamard(const Vector3& first, const Vector3& second);

		float x;
		float y;
		float z;
	};

	struct Vector4
	{
	public:
		Vector4();
		Vector4(float all);
		Vector4(float x, float y, float z);
		Vector4(float x, float y, float z, float w);
		Vector4(const Vector3& base);
		Vector4(const Vector4& other);

		Vector4 operator+(const Vector4& other);
		Vector4 operator+(float all);
		Vector4& operator+=(const Vector4& other);
		Vector4& operator+=(float all);

		Vector4 operator-();
		Vector4 operator-(const Vector4& other);
		Vector4 operator-(float all);
		Vector4& operator-=(const Vector4& other);
		Vector4& operator-=(float all);

		Vector4 operator*(float scalar);
		Vector4& operator*=(float scalar);

		Vector4 operator/(float scalar);
		Vector4& operator/=(float scalar);

		static Vector4 Dot(const Vector4& first, const Vector4& second);
		static Vector4 Cross(const Vector4& first, const Vector4& second);
		static Vector4 Hadamard(const Vector4& first, const Vector4& second);

		float x;
		float y;
		float z;
		float w;
	};
}
}
