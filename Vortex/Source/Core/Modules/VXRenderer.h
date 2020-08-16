#pragma once
#include <Core/Module.h>
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Graphics/Primitives/GPConstantBuffer.h>
#include <Graphics/Primitives/GPVertexShader.h>
#include <Graphics/Primitives/GPPixelShader.h>
#include <Math/Matrix.h>
#include <Core/Events/WindowEvent.h>

DECLARE_LOGGER(LogRenderer);

namespace Vortex
{
	class World;

	class VXRenderer : public Module
	{
	public:
		VXRenderer();
		virtual ~VXRenderer();

		virtual void Startup() override;
		virtual void Shutdown() override;
		
		virtual void Tick(float deltaTime) override;

		virtual void Quit() override;

		virtual void OnConsoleCommand(ConsoleCommand command) override;

		void RenderToFramebuffer(GPFramebuffer* framebuffer);
		void RenderToWindow(Window* window);

		void ResizeFramebuffer(int width, int height);
		bool ResizeOnWindow(WindowResizeEvent& event);

		void RenderWorld(World* worldToRender);

	private:
		struct ConstantBuffer
		{
			Math::Matrix worldViewProjectionMatrix;
		};

		void PostFrambufferResize(int width, int height);

		bool m_IsOnWindow;
		GPFramebuffer* m_Target;

		World* m_World;

		GPVertexShader* m_BasicVertexShader;
		GPPixelShader* m_BasicPixelShader;

		GPConstantBuffer* m_ConstantBuffer;
		ConstantBuffer m_ConstantBufferData;
	};
}

extern Vortex::VXRenderer* GRenderer;
