#pragma once
#include <Math/Vector.h>

namespace Vortex
{
namespace Math
{
	struct Matrix3
	{
	public:
		Matrix3();
		Matrix3(Vector3 row1, Vector3 row2, Vector3 row3);
		Matrix3(Vector3 rows[3]);
		Matrix3(const Matrix3& other);

		Vector3 row1;
		Vector3 row2;
		Vector3 row3;
	};

	struct Matrix4
	{
	public:
		Matrix4();
		Matrix4(Vector4 row1, Vector4 row2, Vector4 row3, Vector4 row4);
		Matrix4(Vector4 rows[4]);
		Matrix4(const Matrix4& other);

		Vector4 row1;
		Vector4 row2;
		Vector4 row3;
		Vector4 row4;
	};
}
}
