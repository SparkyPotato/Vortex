#pragma once

namespace Vortex { class Entity; }

class NativeScript
{
public:
	NativeScript(std::string scriptName);
	virtual ~NativeScript();

	void SetupVariables(Vortex::Entity* owner);
	std::string& GetName() { return m_Name; }

	virtual void Start() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Stop() = 0;

protected:

private:
	std::string m_Name;
	Vortex::Entity* m_Entity = nullptr;
};
