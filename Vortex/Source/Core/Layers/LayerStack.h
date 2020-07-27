#pragma once
#include <vector>
#include <Core/Layers/ILayer.h>

namespace Vortex
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		/*
			Pushes a layer onto the bottom of the layer stack.
			Once pushed in, the layer stack owns all layers and will delete it on destruction.
		*/
		void PushLayer(ILayer* layer);
		/*
			Pops a layer from the layer stack.
			Once popped, it's the application's responsibility to manage the layer.
		*/
		void PopLayer(ILayer* layer);

		/*
			Ticks all owned layers.
		*/
		void Tick(float deltaTime);
		/*
			Passes the event through all layers if it isn't handled.
		*/
		void PassEvent(IEvent& event);
		/*
			Renders ImGui for all layers.
		*/
		void RenderGui();

	private:
		std::vector<ILayer*> m_Layers;
	};
}
