#include <VXpch.h>
#include <ScriptingAPI/NativeScript.h>
#include <World/World.h>

NativeScript::NativeScript(std::string scriptName)
	: m_Name(scriptName)
{

}

NativeScript::~NativeScript()
{

}

void NativeScript::SetupVariables(Vortex::Entity* owner)
{
	m_Entity = owner;
}

void NativeScript::Start()
{

}

void NativeScript::Tick(float deltaTime)
{

}

void NativeScript::Stop()
{

}
