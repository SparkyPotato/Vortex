#include "EditorApp.h"

Vortex::IApplication* CreateApplication()
{
	return new EditorApp();
}

EditorApp::EditorApp()
{
	m_WindowProperties = { "Hello", 800, 600 };
}

EditorApp::~EditorApp()
{

}

const Vortex::IWindow::Properties& EditorApp::GetWindowProperties()
{
	return m_WindowProperties;
}

void EditorApp::Start()
{
	
}

void EditorApp::Tick(float deltaTime)
{
	
}
