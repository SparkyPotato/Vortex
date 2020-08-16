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
		Matrix(Vector row1, Vector row2, Vector row3, Vector row4);
		Matrix(Vector rows[4]);
		Matrix(const Matrix& other);

		/*
			Multiplies two matrices.
		*/
		Matrix operator*(const Matrix& other) const;
		Matrix& operator*=(const Matrix& other);

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
