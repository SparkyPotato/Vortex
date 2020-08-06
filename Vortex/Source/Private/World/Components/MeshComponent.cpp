#include <VXpch.h>
#include <World/Components/MeshComponent.h>

namespace Vortex
{
	MeshComponent::MeshComponent(unsigned int owner)
		: m_Owner(owner)
	{

	}

	MeshComponent::~MeshComponent()
	{
		if (m_Vertices) delete m_Vertices;
		if (m_Indices) delete m_Indices;
	}

	void MeshComponent::LoadFromFile(std::string file)
	{
		if (m_Vertices) delete m_Vertices;
		if (m_Indices) delete m_Indices;


	}

	void MeshComponent::CalculateMatrix(const Math::Vector& position, const Math::Vector& rotation, const Math::Vector& scale)
	{
		using namespace Math;

		m_Transformation = Matrix::Rotate(rotation) * Matrix::Scale(scale) * Matrix::Translate(position);
	}
}
