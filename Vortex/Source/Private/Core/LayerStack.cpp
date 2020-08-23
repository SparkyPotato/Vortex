#include <VXpch.h>
#include <Core/Layers/LayerStack.h>

Vortex::LayerStack* GLayerStack;
DEFINE_LOGGER(LogLayerStack);

namespace Vortex
{
	LayerStack::LayerStack()
	{
		CREATE_LOGGER(LogLayerStack, spdlog::level::trace);
	}

	LayerStack::~LayerStack()
	{
		// Detaches all layers.
		for (auto layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		// Pushes the layer into the vector and attaches it.
		m_Layers.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end())
		{
			// Erases the layer.
			m_Layers.erase(it);
		}
		else
		{
			VX_WARN(LogLayerStack, "Tried to pop layer that wasn't owned by layer stack!");
		}
	}

	void LayerStack::Tick(float deltaTime)
	{
		// Ticks all layers and profiles it.

		ENG_PROFILE("Layers Tick");
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->Tick(deltaTime);
		}
	}

	void LayerStack::PassEvent(Event& event)
	{
		if (event.IsHandled()) return;

		// Goes through each layer and passes the event if it's not handled.
		for (Layer* layer : m_Layers)
		{
			if (!layer) return;

			layer->OnEvent(event);
			if (event.IsHandled()) break;
		}
	}

	void LayerStack::RenderGui()
	{
		// Renders the GUI.
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->OnGuiRender();
		}
	}

	void LayerStack::Render3dUi()
	{
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->On3dUiRender();
		}
	}
}
