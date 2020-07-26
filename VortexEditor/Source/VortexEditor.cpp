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
	frame = Vortex::GPFramebuffer::Create(GCore->GetWindow());
}

void VortexEditor::Tick(float deltaTime)
{
	frame->Bind();
	frame->Clear(1, 0, 0, 1);
}

void VortexEditor::OnEvent(Vortex::IEvent& event)
{
	
}
