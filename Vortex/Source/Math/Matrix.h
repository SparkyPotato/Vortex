#pragma once
#include <Math/Vector.h>

namespace Vortex
{
namespace Math
{
	struct Matrix
	{
	public:
		Matrix();
		Matrix(Vector row1, Vector row2, Vector row3, Vector row4);
		Matrix(Vector rows[4]);
		Matrix(const Matrix& other);

		Matrix operator*(const Matrix& other);
		Matrix& operator*=(const Matrix& other);

		static Matrix Scale(float scaleFactor);
		static Matrix Scale(Vector scaling);

		static Matrix Rotate(Vector rotationDegrees);
		static Matrix Rotate(Vector axis, float angleDegrees);

		static Matrix Translate(Vector translation);

		Vector columns[4];
	};
}
}
