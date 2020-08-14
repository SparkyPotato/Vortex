#include <VXpch.h>
#include <World/Entity.h>
#include <World/World.h>

namespace Vortex
{
	Entity::Entity(unsigned int id, std::string name, World* world)
		: m_ID(id), m_Name(name), m_World(world), m_Transform(m_World->CreateTransformComponent(m_ID))
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
		Entity* child = m_World->CreateEntity(name);
		child->SetParentFromID(m_ID);

		return child;
	}

	Entity* Entity::GetParent()
	{
		return m_World->GetEntityFromID(m_ParentID);
	}

	MeshComponent* Entity::AddMeshComponent()
	{
		return m_World->CreateMeshComponent(m_ID);
	}

	MeshComponent* Entity::GetMeshComponent()
	{
		auto& meshes = m_World->GetMeshes();

		for (MeshComponent& mesh : meshes)
		{
			if (mesh.GetOwnerID() == m_ID)
			{
				return &mesh;
			}
		}

		return nullptr;
	}

	SpriteComponent* Entity::AddSpriteComponent()
	{
		return m_World->CreateSpriteComponent(m_ID);
	}

	SpriteComponent* Entity::GetSpriteComponent()
	{
		auto& sprites = m_World->GetSprites();

		for (SpriteComponent& sprite : sprites)
		{
			if (sprite.GetOwnerID() == m_ID)
			{
				return &sprite;
			}
		}

		return nullptr;
	}

	CameraComponent* Entity::AddCameraComponent()
	{
		return m_World->CreateCameraComponent(m_ID);
	}

	CameraComponent* Entity::GetCameraComponent()
	{
		auto& cameras = m_World->GetCameras();

		for (CameraComponent& camera : cameras)
		{
			if (camera.GetOwnerID() == m_ID)
			{
				return &camera;
			}
		}

		return nullptr;
	}
}
