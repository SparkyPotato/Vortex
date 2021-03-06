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

	Matrix::Matrix(const Vector& row1, const Vector& row2, const Vector& row3, const Vector& row4)
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

	Matrix Matrix::operator*(const Matrix& other) const
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

	Matrix& Matrix::SetColumns(Vector newColumns[4])
	{
		columns[0] = newColumns[0];
		columns[1] = newColumns[1];
		columns[2] = newColumns[2];
		columns[3] = newColumns[3];
		return *this;
	}

	Matrix& Matrix::SetColumns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4)
	{
		columns[0] = column1;
		columns[1] = column2;
		columns[2] = column3;
		columns[3] = column4;
		return *this;
	}

	Matrix Matrix::Columns(Vector columns[4])
	{
		Matrix mat;
		mat.columns[0] = columns[0];
		mat.columns[1] = columns[1];
		mat.columns[2] = columns[2];
		mat.columns[3] = columns[3];
		return mat;
	}

	Matrix Matrix::Columns(const Vector& column1, const Vector& column2, const Vector& column3, const Vector& column4)
	{
		Matrix mat;
		mat.columns[0] = column1;
		mat.columns[1] = column2;
		mat.columns[2] = column3;
		mat.columns[3] = column4;
		return mat;
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
			{ 2.f / width, 0.f,          0.f,                     0.f },
			{ 0.f,         2.f / height, 0.f,                     0.f },
			{ 0.f,         0.f,          1.f / (farp - nearp),    0.f },
			{ 0.f,         0.f,          -nearp / (farp - nearp), 1.f }
		};

		return mat;
	}

	Matrix Matrix::Perspective(float width, float height, float farp, float nearp)
	{
		Matrix mat =
		{
			{ 2.f * nearp / width, 0.f,                  0.f,                              0.f },
			{ 0.f,                 2.f * nearp / height, 0.f,                              0.f },
			{ 0.f,                 0.f,                  farp / (farp - nearp),            1.f },
			{ 0.f,                 0.f,                  -(farp * nearp) / (farp - nearp), 0.f }
		};

		return mat;
	}

	// Please don't read. This is why most people use a library!
	Matrix Matrix::Inverse(const Matrix& m)
	{
		float A2323 = m.columns[2].z * m.columns[3].w - m.columns[2].w * m.columns[3].z;
		float A1323 = m.columns[2].y * m.columns[3].w - m.columns[2].w * m.columns[3].y;
		float A1223 = m.columns[2].y * m.columns[3].z - m.columns[2].z * m.columns[3].y;
		float A0323 = m.columns[2].x * m.columns[3].w - m.columns[2].w * m.columns[3].x;
		float A0223 = m.columns[2].x * m.columns[3].z - m.columns[2].z * m.columns[3].x;
		float A0123 = m.columns[2].x * m.columns[3].y - m.columns[2].y * m.columns[3].x;
		float A2313 = m.columns[1].z * m.columns[3].w - m.columns[1].w * m.columns[3].z;
		float A1313 = m.columns[1].y * m.columns[3].w - m.columns[1].w * m.columns[3].y;
		float A1213 = m.columns[1].y * m.columns[3].z - m.columns[1].z * m.columns[3].y;
		float A2312 = m.columns[1].z * m.columns[2].w - m.columns[1].w * m.columns[2].z;
		float A1312 = m.columns[1].y * m.columns[2].w - m.columns[1].w * m.columns[2].y;
		float A1212 = m.columns[1].y * m.columns[2].z - m.columns[1].z * m.columns[2].y;
		float A0313 = m.columns[1].x * m.columns[3].w - m.columns[1].w * m.columns[3].x;
		float A0213 = m.columns[1].x * m.columns[3].z - m.columns[1].z * m.columns[3].x;
		float A0312 = m.columns[1].x * m.columns[2].w - m.columns[1].w * m.columns[2].x;
		float A0212 = m.columns[1].x * m.columns[2].z - m.columns[1].z * m.columns[2].x;
		float A0113 = m.columns[1].x * m.columns[3].y - m.columns[1].y * m.columns[3].x;
		float A0112 = m.columns[1].x * m.columns[2].y - m.columns[1].y * m.columns[2].x;

		float det = m.columns[0].x * (m.columns[1].y * A2323 - m.columns[1].z * A1323 + m.columns[1].w * A1223)
					- m.columns[0].y * (m.columns[1].x * A2323 - m.columns[1].z * A0323 + m.columns[1].w * A0223)
					+ m.columns[0].z * (m.columns[1].x * A1323 - m.columns[1].y * A0323 + m.columns[1].w * A0123)
					- m.columns[0].w * (m.columns[1].x * A1223 - m.columns[1].y * A0223 + m.columns[1].z * A0123);
		det = 1.f / det;

		Matrix mat;

		mat.columns[0].x = det * (m.columns[1].y * A2323 - m.columns[1].z * A1323 + m.columns[1].w * A1223);
		mat.columns[0].y = det * -(m.columns[0].y * A2323 - m.columns[0].z * A1323 + m.columns[0].w * A1223);
		mat.columns[0].z = det * (m.columns[0].y * A2313 - m.columns[0].z * A1313 + m.columns[0].w * A1213);
		mat.columns[0].w = det * -(m.columns[0].y * A2312 - m.columns[0].z * A1312 + m.columns[0].w * A1212);
		mat.columns[1].x = det * -(m.columns[1].x * A2323 - m.columns[1].z * A0323 + m.columns[1].w * A0223);
		mat.columns[1].y = det * (m.columns[0].x * A2323 - m.columns[0].z * A0323 + m.columns[0].w * A0223);
		mat.columns[1].z = det * -(m.columns[0].x * A2313 - m.columns[0].z * A0313 + m.columns[0].w * A0213);
		mat.columns[1].w = det * (m.columns[0].x * A2312 - m.columns[0].z * A0312 + m.columns[0].w * A0212);
		mat.columns[2].x = det * (m.columns[1].x * A1323 - m.columns[1].y * A0323 + m.columns[1].w * A0123);
		mat.columns[2].y = det * -(m.columns[0].x * A1323 - m.columns[0].y * A0323 + m.columns[0].w * A0123);
		mat.columns[2].z = det * (m.columns[0].x * A1313 - m.columns[0].y * A0313 + m.columns[0].w * A0113);
		mat.columns[2].w = det * -(m.columns[0].x * A1312 - m.columns[0].y * A0312 + m.columns[0].w * A0112);
		mat.columns[3].x = det * -(m.columns[1].x * A1223 - m.columns[1].y * A0223 + m.columns[1].z * A0123);
		mat.columns[3].y = det * (m.columns[0].x * A1223 - m.columns[0].y * A0223 + m.columns[0].z * A0123);
		mat.columns[3].z = det * -(m.columns[0].x * A1213 - m.columns[0].y * A0213 + m.columns[0].z * A0113);
		mat.columns[3].w = det * (m.columns[0].x * A1212 - m.columns[0].y * A0212 + m.columns[0].z * A0112);

		return mat;
	}
}
}
 