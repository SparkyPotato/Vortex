#include <VXpch.h>
#include <Math/Matrix.h>
#include <math.h>

namespace Vortex
{
namespace Math
{
	constexpr double PI = 3.14159265;

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

	Matrix Matrix::Scale(float scaleFactor)
	{
		Matrix mat =
		{
			{ scaleFactor, 0.f,         0.f,         0.f },
			{ 0.f,         scaleFactor, 0.f,         0.f },
			{ 0.f,         0.f,         scaleFactor, 0.f },
			{ 0.f,         0.f,         0.f,         1.f }
		};

		return mat;
	}

	Matrix Matrix::Scale(Vector scaling)
	{
		Matrix mat =
		{
			{ scaling.x, 0.f,       0.f,       0.f },
			{ 0.f,       scaling.y, 0.f,       0.f },
			{ 0.f,       0.f,       scaling.z, 0.f },
			{ 0.f,       0.f,       0.f,       1.f }
		};

		return mat;
	}

	Matrix Matrix::Rotate(Vector rotation)
	{
		float sinx = (float) std::sin(-rotation.x * PI / 180.f);
		float cosx = (float) std::cos(-rotation.x * PI / 180.f);
		float siny = (float) std::sin(-rotation.y * PI / 180.f);
		float cosy = (float) std::cos(-rotation.y * PI / 180.f);
		float sinz = (float) std::sin(-rotation.z * PI / 180.f);
		float cosz = (float) std::cos(-rotation.z * PI / 180.f);

		Matrix rotX =
		{
			{ 1.f, 0.f,   0.f,  0.f },
			{ 0.f, cosx,  sinx, 0.f },
			{ 0.f, -sinx, cosx, 0.f },
			{ 0.f, 0.f,   0.f,  1.f }
		};
		Matrix rotY =
		{
			{ cosy, 0.f, -siny, 0.f },
			{ 0.f,  1.f, 0.f,   0.f },
			{ siny, 0.f, cosy,  0.f },
			{ 0.f,  0.f, 0.f,   1.f }
		};
		Matrix rotZ =
		{
			{ cosz,  sinz, 0.f, 0.f },
			{ -sinz, cosz, 0.f, 0.f },
			{ 0.f,   0.f,  1.f, 0.f },
			{ 0.f,   0.f,  0.f, 1.f }
		};

		return rotX * rotY * rotZ;
	}

	Matrix Matrix::Rotate(Vector axis, float angleDegrees)
	{
		Vector vec = axis.GetNormalized();
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;
		float cos = (float) std::cos(-angleDegrees * PI / 180.f);
		float sin = (float) std::sin(-angleDegrees * PI / 180.f);

		Matrix mat =
		{
			{ cos + (x * x) * (1 - cos),    (x * y) * (1 - cos) + z * sin,  (x * z) * (1 - cos) - y * sin, 0.f },
			{ (x * y) * (1 - cos) - z * sin, cos + (y * y) * (1 - cos),     (y * z) * (1 - cos) + x * sin, 0.f },
			{ (x * z) * (1 - cos) + y * sin, (y * z) * (1 - cos) - x * sin, cos + (z * z) * (1 - cos),     0.f },
			{ 0.f,                           0.f,                           0.f,                           1.f }
		};

		return mat;
	}

	Matrix Matrix::Translate(Vector translation)
	{
		Matrix mat =
		{
			{ 1.f,           0.f,           0.f,           0.f },
			{ 0.f,           1.f,           0.f,           0.f },
			{ 0.f,           0.f,           1.f,           0.f },
			{ translation.x, translation.y, translation.z, 1.f }
		};

		return mat;
	}

	Matrix Matrix::Orthographic(float width, float height, float farp, float nearp)
	{
		Matrix mat =
		{
			{ 2.f / width, 0.f,          0.f,                          0.f },
			{ 0.f,         2.f / height, 0.f,                          0.f },
			{ 0.f,         0.f,          -2.f / (farp - nearp),          0.f },
			{ 0.f,         0.f,          -(farp + nearp) / (farp - nearp), 1.f }
		};

		return mat;
	}
}
}
 