#pragma once
#include <Graphics/Primitives/GraphicsPrimitive.h>

namespace Vortex
{
	enum class GraphicsAPI
	{
		None, DirectX11, DirectX12, Vulkan
	};

	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() {}

		static void Create(GraphicsAPI api);
		static void Recreate();
		static IGraphicsContext* Get() { return s_Context; }

		GraphicsAPI GetAPI() { return m_API; }
		void RegisterPrimitive(GraphicsPrimitive* primitive);
		void UnregisterPrimitive(GraphicsPrimitive* primitive);

	private:
		static IGraphicsContext* s_Context;

		GraphicsAPI m_API;
		std::vector<GraphicsPrimitive*> m_Primitives;
	};
}
