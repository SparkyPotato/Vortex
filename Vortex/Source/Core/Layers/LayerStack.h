#pragma once
#include <Core/Layers/Layer.h>

DECLARE_LOGGER(LogLayerStack);

namespace Vortex
{
	/*
		A collection of layers.
		Deals with event propogation and ticking.
	*/
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		/*
			Pushes a layer onto the bottom of the layer stack.
			Once pushed in, the layer stack owns all layers and will delete it on destruction.
		*/
		void PushLayer(Layer* layer);
		/*
			Pops a layer from the layer stack.
			Once popped, it's the application's responsibility to manage the layer.
		*/
		void PopLayer(Layer* layer);

		/*
			Ticks all owned layers.
		*/
		void Tick(float deltaTime);
		/*
			Passes the event through all layers if it isn't handled.
		*/
		void PassEvent(Event& event);

		/*
			Renders ImGui for all layers.
		*/
		void RenderGui();
		/*
			Renders Im3d for all layers.
		*/
		void Render3dUi();

	private:
		std::vector<Layer*> m_Layers;
	};
}

extern Vortex::LayerStack* GLayerStack;
