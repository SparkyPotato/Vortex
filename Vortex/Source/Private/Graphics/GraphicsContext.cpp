#include <VXpch.h>
#include <Graphics/GraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#endif

DEFINE_LOGGER(LogGraphicsAPI);

namespace Vortex
{
	GraphicsContext* GraphicsContext::s_Context = nullptr;

	void GraphicsContext::Create(GraphicsAPI api)
	{
		// Throw an exception if the Context has already been created.
		if (s_Context) throw std::exception("Graphics context has already been created!");

		CREATE_LOGGER(LogGraphicsAPI, spdlog::level::trace);

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

	void GraphicsContext::Destroy()
	{
		// If all primitives have not been deleted...
		if (s_Context->m_Primitives.size() != 0)
		{
			// ... Throw a warning.
			VX_ERROR(LogGraphicsAPI, "{0} Graphics Primitives have not been deleted.", s_Context->m_Primitives.size());
			VX_ERROR(LogGraphicsAPI, "Deleting...");

			// ... And delete all.
			while (!s_Context->m_Primitives.empty())
			{
				delete* (s_Context->m_Primitives.begin());
			}
		}

		delete s_Context;
		s_Context = nullptr;
	}

	void GraphicsContext::Recreate()
	{
		// Recreation isn't handled as of now.

// 		GraphicsAPI api = s_Context->GetAPI();
// 		std::vector<std::unique_ptr<GraphicsPrimitive>> primitives = s_Context->m_Primitives;
// 		delete s_Context;
// 
// 		Create(api);
// 		s_Context->m_Primitives = primitives;
// 
// 		// Recreate all primitives in the list.
// 		for (GraphicsPrimitive* primitive : s_Context->m_Primitives)
// 		{
// 			if (primitive)
// 				primitive->Recreate();
// 			else
// 				ENG_ERROR("Primitive has not been registered!");
// 		}
	}

	void GraphicsContext::RegisterPrimitive(GraphicsPrimitive* primitive)
	{
		VX_TRACE(LogGraphicsAPI, "Registered primitive.");
		m_Primitives.push_back(primitive);
	}

	void GraphicsContext::UnregisterPrimitive(GraphicsPrimitive* primitive)
	{
		auto it = std::find(m_Primitives.begin(), m_Primitives.end(), primitive);

		if (it != m_Primitives.end())
		{
			m_Primitives.erase(it);
			VX_TRACE(LogGraphicsAPI, "Unregistered primitive.");
		}
		else
		{
			VX_ERROR(LogGraphicsAPI, "Trying to unregister primitive that was never registered!");
		}
	}
}
