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

		Vector columns[4];
	};
}
}
