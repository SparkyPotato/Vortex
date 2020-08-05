#pragma once

namespace Vortex
{
	class World;
	class Entity;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		void SetOwner(unsigned int entityID) { m_Owner = entityID; }
		void SetWorld(World* world) { m_World = world; }

		virtual void Start() = 0;
		virtual void Tick(float deltaTime) = 0;
		virtual void End() = 0;

	protected:
		World* GetWorld() { return m_World; }
		Entity* GetOwner();
		unsigned int GetOwnerID() { return m_Owner; }

	private:
		unsigned int m_Owner = 0;
		World* m_World = nullptr;
	};
}
