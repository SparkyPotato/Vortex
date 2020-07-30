#include <VXpch.h>
#include <Math/Matrix.h>

namespace Vortex
{
namespace Math
{
	Matrix4::Matrix4()
	{
		row1 = { 1.f, 0.f, 0.f, 0.f };
		row2 = { 0.f, 1.f, 0.f, 0.f };
		row3 = { 0.f, 0.f, 1.f, 0.f };
		row4 = { 0.f, 0.f, 0.f, 1.f };
	}

	Matrix4::Matrix4(Vector4 row1v, Vector4 row2v, Vector4 row3v, Vector4 row4v)
	{
		row1 = row1v;
		row2 = row2v;
		row3 = row3v;
		row4 = row4v;
	}

	Matrix4::Matrix4(Vector4 rows[4])
	{
		row1 = rows[0];
		row2 = rows[1];
		row3 = rows[2];
		row4 = rows[3];
	}

	Matrix4::Matrix4(const Matrix4& other)
	{
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
		row4 = other.row4;
	}
}
}
