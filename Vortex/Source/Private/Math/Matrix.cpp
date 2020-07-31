#include <VXpch.h>
#include <Math/Matrix.h>

namespace Vortex
{
namespace Math
{
	Matrix::Matrix()
	{
		columns[0] = { 1.f, 0.f, 0.f, 0.f };
		columns[1] = { 0.f, 1.f, 0.f, 0.f };
		columns[2] = { 0.f, 0.f, 1.f, 0.f };
		columns[3] = { 0.f, 0.f, 0.f, 1.f };
	}

	Matrix::Matrix(Vector row1, Vector row2, Vector row3, Vector row4)
	{
		columns[0].x = row1.x;
		columns[1].x = row1.y;
		columns[2].x = row1.z;
		columns[3].x = row1.w;

		columns[0].y = row2.x;
		columns[1].y = row2.y;
		columns[2].y = row2.z;
		columns[3].y = row2.w;

		columns[0].z = row3.x;
		columns[1].z = row3.y;
		columns[2].z = row3.z;
		columns[3].z = row3.w;

		columns[0].w = row4.x;
		columns[1].w = row4.y;
		columns[2].w = row4.z;
		columns[3].w = row4.w;
	}

	Matrix::Matrix(Vector rows[4])
	{
		columns[0].x = rows[0].x;
		columns[1].x = rows[0].y;
		columns[2].x = rows[0].z;
		columns[3].x = rows[0].w;

		columns[0].y = rows[1].x;
		columns[1].y = rows[1].y;
		columns[2].y = rows[1].z;
		columns[3].y = rows[1].w;

		columns[0].z = rows[2].x;
		columns[1].z = rows[2].y;
		columns[2].z = rows[2].z;
		columns[3].z = rows[2].w;

		columns[0].w = rows[3].x;
		columns[1].w = rows[3].y;
		columns[2].w = rows[3].z;
		columns[3].w = rows[3].w;
	}

	Matrix::Matrix(const Matrix& other)
	{
		columns[0] = other.columns[0];
		columns[1] = other.columns[1];
		columns[2] = other.columns[2];
		columns[3] = other.columns[3];
	}

	Matrix Matrix::operator*(const Matrix& other)
	{
		Matrix mat;

		for (int i = 0; i < 4; i++)
		{
			Vector row(columns[0].x, columns[1].x, columns[2].x, columns[3].x);
			mat.columns[i].x = Vector::Dot4D(row, other.columns[i]);

			row = { columns[0].y, columns[1].y, columns[2].y, columns[3].y };
			mat.columns[i].y = Vector::Dot4D(row, other.columns[i]);

			row = { columns[0].z, columns[1].z, columns[2].z, columns[3].z };
			mat.columns[i].z = Vector::Dot4D(row, other.columns[i]);

			row = { columns[0].w, columns[1].w, columns[2].w, columns[3].w };
			mat.columns[i].w = Vector::Dot4D(row, other.columns[i]);
		}

		return mat;
	}

	Matrix& Matrix::operator*=(const Matrix& other)
	{
		*this = *this * other;

		return *this;
	}
}
}
