#pragma once
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPIndexBuffer.h>
#include <Math/Matrix.h>
#include <World/Components/TransformComponent.h>

namespace Vortex
{
	class World;

	class Grid
	{
	public:
		Grid(float sideLength, float largeLineDistance, float smallLineDistance, const Math::Vector& color);
		~Grid();

		GPVertexBuffer* vertices = nullptr;
		GPIndexBuffer* indices = nullptr;

	private:
		struct Vertex
		{
			Math::Vector position;
			Math::Vector color;
		};
	};
}
