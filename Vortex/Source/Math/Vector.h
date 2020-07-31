#pragma once

namespace Vortex
{
namespace Math
{
	struct Matrix;

	struct Vector
	{
	public:
		Vector();
		Vector(float all);
		Vector(float x, float y, float z);
		Vector(float x, float y, float z, float w);
		Vector(const Vector& other);

		Vector operator+(const Vector& other);
		Vector operator+(float all);
		Vector& operator+=(const Vector& other);
		Vector& operator+=(float all);

		Vector operator-();
		Vector operator-(const Vector& other);
		Vector operator-(float all);
		Vector& operator-=(const Vector& other);
		Vector& operator-=(float all);

		Vector operator*(float scalar);
		Vector& operator*=(float scalar);
		Vector operator*(const Matrix& transform);
		Vector& operator*=(const Matrix& transform);

		Vector operator/(float scalar);
		Vector& operator/=(float scalar);

		float GetLength();

		Vector& Normalize();
		Vector GetNormalized();

		static float Dot3D(const Vector& first, const Vector& second);
		static Vector Cross3D(const Vector& first, const Vector& second);
		static Vector Hadamard3D(const Vector& first, const Vector& second);

		static float Dot4D(const Vector& first, const Vector& second);
		static Vector Hadamard4D(const Vector& first, const Vector& second);

		float x;
		float y;
		float z;
		float w;
	};
}
}
