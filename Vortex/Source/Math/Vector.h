#pragma once

namespace Vortex
{
namespace Math
{
	struct Matrix;

	/*
		A 4x1 row vecotr.
	*/
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

		/*
			Gets the length of the vector.
		*/
		float GetLength();

		/*
			Normalizes the vector in-place and returns the normalized vector.
		*/
		Vector& Normalize();
		/*
			Gets the normalized vector.
		*/
		Vector GetNormalized();

		/*
			Three dimensional vector products.
		*/
		static float Dot3D(const Vector& first, const Vector& second);
		static Vector Cross3D(const Vector& first, const Vector& second);
		static Vector Hadamard3D(const Vector& first, const Vector& second);

		/*
			Four dimensional vector products.
		*/
		static float Dot4D(const Vector& first, const Vector& second);
		static Vector Hadamard4D(const Vector& first, const Vector& second);

		// Individual components.
		float x;
		float y;
		float z;
		float w;
	};
}
}
