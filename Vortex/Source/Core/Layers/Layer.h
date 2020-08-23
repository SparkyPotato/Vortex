#pragma once
#include <Core/Event.h>

namespace Vortex
{
	/*
		Defines a layer of an application. 
		Is stored in the global GLayerStack, but a layer can have its own layer stack as well.
	*/
	class Layer
	{
	public:
		virtual ~Layer() {}

		/*
			Called when the layer is attached to a layer stack.
		*/
		virtual void OnAttach() = 0;
		/*
			Called when the layer is detached from a layer stack.
		*/
		virtual void OnDetach() = 0;

		/*
			Called every frame by the layer stack.
		*/
		virtual void Tick(float deltaTime) = 0;

		/*
			Called every frame when ImGui is being rendered.
		*/
		virtual void OnGuiRender() {}

		/*
			Called every frame when Im3d is being rendered.
		*/
		virtual void On3dUiRender() {}
		/*
			Called on all unhandled events from layers above it on the layer stack.
			Doesn't receive events handled by the Vortex Core.
		*/
		virtual void OnEvent(Event& event) {}
	};
}
