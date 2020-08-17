#include <VXpch.h>
#include <World/Entity.h>
#include <World/World.h>

namespace Vortex
{
	Entity::Entity(unsigned int id, std::string name, World* world)
		: m_ID(id), m_Name(name), m_World(world)
	{
		
	}

	Entity::~Entity()
	{

	}

	bool Entity::operator==(const Entity& other)
	{
		return m_ID == other.m_ID;
	}

	bool Entity::operator==(unsigned int id)
	{
		return m_ID == id;
	}

	Entity* Entity::AddChild(std::string name)
	{
		return m_World->CreateChildEntity(this, name);
	}

	Entity* Entity::GetParent()
	{
		return m_World->GetEntityFromID(m_ParentID);
	}

	void Entity::SetParent(Entity* parent)
	{
		m_ParentID = parent->GetID();
		m_World->SetEntityParent(this, parent);
	}

	void Entity::SetParentFromID(unsigned int parentID)
	{
		m_ParentID = parentID;
		m_World->SetEntityParent(this, m_World->GetEntityFromID(parentID));
	}

	MeshComponent* Entity::AddMeshComponent()
	{
		return m_Mesh = m_World->CreateMeshComponent(m_ID);
	}

	MeshComponent* Entity::GetMeshComponent()
	{
		return m_Mesh;
	}

	SpriteComponent* Entity::AddSpriteComponent(float width, float height, Math::Matrix colors)
	{
		return  m_Sprite = m_World->CreateSpriteComponent(m_ID);
	}

	SpriteComponent* Entity::GetSpriteComponent()
	{
		return m_Sprite;
	}

	CameraComponent* Entity::AddCameraComponent()
	{
		return m_Camera = m_World->CreateCameraComponent(m_ID);
	}

	CameraComponent* Entity::GetCameraComponent()
	{
		return m_Camera;
	}
}
