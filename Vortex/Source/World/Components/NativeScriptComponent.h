#pragma once

namespace Vortex
{
	class World;

	class NativeScriptComponent
	{
	public:
		NativeScriptComponent(unsigned int owner, World* world);
		~NativeScriptComponent();

		unsigned int GetOwnerID() { return m_Owner; }

		void StartAllScripts();
		void TickAllScripts(float deltaTime);
		void StopAllScripts();

		template<typename T>
		void AddScript()
		{

		}

	private:
		unsigned int m_Owner;
		World* m_World;
	};
}
