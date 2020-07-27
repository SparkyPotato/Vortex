#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	/*
		The Render API that the Graphics Context is using.
	*/
	enum class GraphicsAPI
	{
		None, DirectX11, DirectX12, Vulkan
	};

	/*
		Singleton base class for all API-specific implementations. 
		All GraphicsPrimitives have to be written for each API as well.
	*/
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() {}
		/*
			Creates the singleton with the API given.
		*/
		static void Create(GraphicsAPI api);
		/*
			Destroys the Graphics Context and ensures all primitives are deleted properly.
		*/
		static void Destroy();
		/*
			Recreates Context and all primitives. 
			Is usually done if the driver has been updated or if the device was removed.
		*/
		static void Recreate();
		/*
			Getter for the singleton. 
			Can be nullptr if the Context wasn't created yet!
		*/
		static IGraphicsContext* Get() { return s_Context; }

		/*
			Returns the current Render API.
		*/
		GraphicsAPI GetAPI() { return m_API; }

		/*
			Memory management functions. 
			Have to be called in every Graphics Primitive's constructor and destructors.
		*/
		void RegisterPrimitive(GraphicsPrimitive* primitive);
		void UnregisterPrimitive(GraphicsPrimitive* primitive);

	private:
		static IGraphicsContext* s_Context;

		GraphicsAPI m_API;
		// Holds every primitive currently registered with the Graphics Context.
		std::vector<GraphicsPrimitive*> m_Primitives;
	};
}
