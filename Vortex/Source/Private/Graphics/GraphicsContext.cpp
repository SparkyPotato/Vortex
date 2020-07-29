#include <VXpch.h>
#include <Graphics/GraphicsContext.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#endif


namespace Vortex
{
	GraphicsContext* GraphicsContext::s_Context = nullptr;

	void GraphicsContext::Create(GraphicsAPI api)
	{
		// Throw an exception if the Context has already been created.
		if (s_Context) throw std::exception("Graphics context has already been created!");

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
			// ... Throw a warning...
			ENG_WARN("{0} Graphics Primitives have not been deleted.", s_Context->m_Primitives.size());
			ENG_WARN("Deleting...");

			// ... And delete them.
			size_t size = s_Context->m_Primitives.size();

			for (int i = 0; i < size; i++)
			{
				delete s_Context->m_Primitives[0];
			}

			// Sleep for 10 seconds so the user knows something is wrong and has time to read the warnings.
			ENG_WARN("Sleeping for 10 seconds before continuing...");
			Sleep(10000);
		}

		delete s_Context;
		s_Context = nullptr;
	}

	void GraphicsContext::Recreate()
	{
		GraphicsAPI api = s_Context->GetAPI();
		std::vector<GraphicsPrimitive*> primitives = s_Context->m_Primitives;
		delete s_Context;

		Create(api);
		s_Context->m_Primitives = primitives;

		// Recreate all primitives in the list.
		for (GraphicsPrimitive* primitive : s_Context->m_Primitives)
		{
			if (primitive)
				primitive->Recreate();
			else
				ENG_ERROR("Primitive has not been registered!");
		}
	}

	void GraphicsContext::RegisterPrimitive(GraphicsPrimitive* primitive)
	{
		ENG_TRACE("Registered primitive.");
		m_Primitives.push_back(primitive);
	}

	void GraphicsContext::UnregisterPrimitive(GraphicsPrimitive* primitive)
	{
		ENG_TRACE("Unregistered primitive.");
		auto i = std::find(s_Context->m_Primitives.begin(), s_Context->m_Primitives.end(), primitive);
		if (i != m_Primitives.end())
			s_Context->m_Primitives.erase(i);
		else
			throw std::exception("Tried to unregister graphics primitive that wasn't registed.");
	}
}
