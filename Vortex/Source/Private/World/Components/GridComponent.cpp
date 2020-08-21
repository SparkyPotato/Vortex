#include <VXpch.h>
#include <World/Components/GridComponent.h>

namespace Vortex
{
	Grid::Grid(float sideLength, float largeLineDistance, float smallLineDistance, const Math::Vector& color)
	{
		float xPos = -sideLength / 2.f, zPos = -sideLength / 2.f;

		int vertexCount = (int) (sideLength / largeLineDistance + 1) * 4;
		auto vertexArray = new Vertex[vertexCount];
		auto indexArray = new unsigned int[vertexCount * 2];

		unsigned int i = 0;
		for (; xPos <= sideLength / 2.f; xPos += largeLineDistance)
		{
			vertexArray[i].position = { xPos, 0.f, -sideLength / 2.f };
			vertexArray[i].color = color;
			indexArray[i] = i;
			i++;

			vertexArray[i].position = { xPos, 0.f, sideLength / 2.f };
			vertexArray[i].color = color;
			indexArray[i] = i;
			i++;
		}

		for (; zPos <= sideLength / 2.f; zPos += largeLineDistance)
		{
			vertexArray[i].position = { -sideLength / 2.f, 0.f, zPos };
			vertexArray[i].color = color;
			indexArray[i] = i;
			i++;

			vertexArray[i].position = { sideLength / 2.f, 0.f, zPos };
			vertexArray[i].color = color;
			indexArray[i] = i;
			i++;
		}

		VertexLayout layout =
		{
			{ "POSITION", ShaderDataType::float4 },
			{ "COLOR", ShaderDataType::float4 }
		};

		vertices = GPVertexBuffer::Create(vertexArray, vertexCount, layout, BufferAccessType::Static);
		indices = GPIndexBuffer::Create(indexArray, vertexCount * 2);

		delete[] vertexArray;
		delete[] indexArray;
	}

	Grid::~Grid()
	{
		delete vertices;
		delete indices;
	}
}
