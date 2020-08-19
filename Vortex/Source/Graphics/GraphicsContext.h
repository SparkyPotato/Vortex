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
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() {}
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
		static GraphicsContext* Get() { return s_Context; }

		/*
			Returns the current Render API.
		*/
		GraphicsAPI GetAPI() { return m_API; }

		/*
			Memory management functions. 
			Have to be called in every Graphics Primitive's constructor and destructor.
		*/
		void RegisterPrimitive(GraphicsPrimitive* primitive);
		void UnregisterPrimitive(GraphicsPrimitive* primitive);

		/*
			Draws to the screen.
		*/
		virtual void Draw(int drawSize) = 0;

		/*
			Threading functions. 
			Please call if you're accessing implementation-specific stuff.
			(reinterpret_casting GraphicsContext::Get()).
			It is handled automatically when calling API-independent functions (Like Draw).
		*/
		virtual void Lock() = 0;
		virtual void Unlock() = 0;

	private:
		static GraphicsContext* s_Context;

		GraphicsAPI m_API;
		// Holds every primitive currently registered with the Graphics Context.
		std::vector<GraphicsPrimitive*> m_Primitives;
	};
}
