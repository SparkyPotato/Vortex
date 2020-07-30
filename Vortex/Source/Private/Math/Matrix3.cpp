#include <VXpch.h>
#include <Math/Matrix.h>

namespace Vortex
{
namespace Math
{
	Matrix3::Matrix3()
	{
		row1 = { 1.f, 0.f, 0.f };
		row2 = { 0.f, 1.f, 0.f };
		row3 = { 0.f, 0.f, 1.f };
	}

	Matrix3::Matrix3(Vector3 row1v, Vector3 row2v, Vector3 row3v)
	{
		row1 = row1v;
		row2 = row2v;
		row3 = row3v;
	}

	Matrix3::Matrix3(Vector3 rows[3])
	{
		row1 = rows[0];
		row2 = rows[1];
		row3 = rows[2];
	}

	Matrix3::Matrix3(const Matrix3& other)
	{
		row1 = other.row1;
		row2 = other.row2;
		row3 = other.row3;
	}
}
}
