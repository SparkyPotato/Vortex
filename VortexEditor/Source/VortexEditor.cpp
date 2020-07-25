#include "VortexEditor.h"

Vortex::IApplication* CreateApplication()
{
	return new VortexEditor();
}

VortexEditor::VortexEditor()
{
	
}

VortexEditor::~VortexEditor()
{

}

void VortexEditor::Start()
{
	
}

void VortexEditor::Tick(float deltaTime)
{
	if (GCore->GetInput()->IsKeyDown(Vortex::InputCode::DownArrow))
	{
		VX_TRACE("Down Arrow Pressed");
	}
}

void VortexEditor::OnEvent(Vortex::IEvent& event)
{
	
}
