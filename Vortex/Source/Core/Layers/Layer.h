#pragma once
#include <Core/Event.h>

namespace Vortex
{
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
			Called on all unhandled events from layers above it on the layer stack.
			Doesn't recieve events handled by the Vortex Core.
		*/
		virtual void OnEvent(Event& event) {}
	};
}
