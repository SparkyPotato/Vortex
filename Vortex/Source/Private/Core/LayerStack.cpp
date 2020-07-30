#include <VXpch.h>
#include <Core/Layers/LayerStack.h>

namespace Vortex
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace_back(layer);
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
		else
		{
			ENG_WARN("Tried to pop layer that wasn't owned by layer stack!");
		}
	}

	void LayerStack::Tick(float deltaTime)
	{
		ENG_PROFILE("Layers Tick");
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->Tick(deltaTime);
		}
	}

	void LayerStack::PassEvent(Event& event)
	{
		if (event.IsHandled()) return;

		for (Layer* layer : m_Layers)
		{
			if (!layer) return;

			layer->OnEvent(event);
			if (event.IsHandled()) break;
		}
	}

	void LayerStack::RenderGui()
	{
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->OnGuiRender();
		}
	}
}
