#pragma once
#include <Math/Vector.h>

namespace Vortex
{
namespace Math
{
	/*
		A 4x4 column-major matrix.
	*/
	struct Matrix
	{
	public:
		Matrix();
		Matrix(const Vector& row1, const Vector& row2, const Vector& row3, const Vector& row4);
		Matrix(Vector rows[4]);
		Matrix(const Matrix& other);

		/*
			Multiplies two matrices.
		*/
		Matrix operator*(const Matrix& other) const;
		Matrix& operator*=(const Matrix& other);

		Matrix& SetColumns(Vector columns[4]);
		Matrix& SetColumns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4);

		static Matrix Columns(Vector columns[4]);
		static Matrix Columns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4);

		/*
			Generates a scaling matrix.
		*/
		static Matrix Scale(float scaleFactor);
		static Matrix Scale(Vector scaling);

		/*
			Creates a rotation matrix.
		*/
		static Matrix Rotate(Vector rotationDegrees);
		static Matrix Rotate(Vector axis, float angleDegrees);

		/*
			Creates a translation matrix.
		*/
		static Matrix Translate(Vector translation);

		/*
			Creates an orthographic projection matrix.
		*/
		static Matrix Orthographic(float width, float height, float farPlane, float nearPlane);
		/*
			Creates a perspective projection matrix..
		*/
		static Matrix Perspective(float width, float height, float farPlane, float nearPlane);

		static Matrix Inverse(const Matrix& matrixToInvert);

		// All the columns of the matrix.
		Vector columns[4];
	};

	template<typename OStream>
	OStream& operator<<(OStream& os, const Matrix& mat)
	{
		return os << "Columns: \n"
				  << mat.columns[0] << "\n"
				  << mat.columns[1] << "\n"
				  << mat.columns[2] << "\n"
				  << mat.columns[3] << "\n";
	}
}
}
