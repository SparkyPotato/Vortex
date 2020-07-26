#include <VXpch.h>
#include <Graphics/IGraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#endif


namespace Vortex
{
	IGraphicsContext* IGraphicsContext::s_Context = nullptr;

	void IGraphicsContext::Create(GraphicsAPI api)
	{
		switch (api)
		{
		case GraphicsAPI::None:
			throw std::exception("Graphics API None is not available.");
			break;
		case GraphicsAPI::DirectX11:
			#ifdef PLATFORM_WINDOWS
			s_Context = new DX11GraphicsContext();
			s_Context->m_API = GraphicsAPI::DirectX11;
			#else
			throw std::exception("DirectX is only supported on Windows.");
			#endif
			break;
		case GraphicsAPI::DirectX12:
			throw std::exception("Graphics API DirectX 12 is not available.");
			break;
		case GraphicsAPI::Vulkan:
			throw std::exception("Graphics API Vulkan is not available.");
			break;
		}
	}

	void IGraphicsContext::Destroy()
	{
		delete s_Context;
	}

	void IGraphicsContext::Recreate()
	{
		GraphicsAPI api = s_Context->GetAPI();
		std::vector<GraphicsPrimitive*> primitives = s_Context->m_Primitives;
		delete s_Context;

		Create(api);
		s_Context->m_Primitives = primitives;
		for (GraphicsPrimitive* primitive : s_Context->m_Primitives)
		{
			if (primitive)
				primitive->Recreate();
			else
				s_Context->UnregisterPrimitive(primitive);
		}
	}

	void IGraphicsContext::RegisterPrimitive(GraphicsPrimitive* primitive)
	{
		ENG_TRACE("Registered primitive.");
		m_Primitives.push_back(primitive);
	}

	void IGraphicsContext::UnregisterPrimitive(GraphicsPrimitive* primitive)
	{
		ENG_TRACE("Unregistered primitive.");
		auto i = std::find(s_Context->m_Primitives.begin(), s_Context->m_Primitives.end(), primitive);
		if (i != m_Primitives.end())
			s_Context->m_Primitives.erase(i);
		else
			throw std::exception("Tried to unregister graphics primitive that wasn't registed.");
	}
}
