#include <VXpch.h>
#include <Core/Layers/LayerStack.h>

namespace Vortex
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		size_t size = m_Layers.size();

		for (int i = 0; i < size; i++)
		{
			delete m_Layers[0];
			m_Layers[0] = nullptr;
		}
	}

	void LayerStack::PushLayer(ILayer* layer)
	{
		m_Layers.push_back(layer);
	}

	void LayerStack::PopLayer(ILayer* layer)
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
		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); std::advance(it, 1))
		{
			(*it)->Tick(deltaTime);
		}
	}

	void LayerStack::PassEvent(IEvent& event)
	{
		if (event.IsHandled()) return;

		for (ILayer* layer : m_Layers)
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
