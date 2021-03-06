#pragma once
#include <Core/Module.h>
#include <Graphics/Primitives/GPVertexBuffer.h>
#include <Graphics/Primitives/GPFramebuffer.h>
#include <Graphics/Primitives/GPConstantBuffer.h>
#include <Graphics/Primitives/GPVertexShader.h>
#include <Graphics/Primitives/GPGeometryShader.h>
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

		GPFramebuffer* GetTarget() { return m_Target; }

		void RequestFramebufferResize(int width, int height);
		void ResizeAllCameras();
		void ResizeIfRequired();
		bool ResizeOnWindow(WindowResizeEvent& event);

		void RenderWorld(World* worldToRender);

		World* GetWorld() { return m_World; }

	private:
		void PostFrambufferResize(int width, int height);
		void ResizeFramebuffer(int width, int height);

		struct ConstantBuffer
		{
			Math::Matrix worldViewProjectionMatrix;
		};

		bool m_ShouldResize = false;
		int m_Width, m_Height = 0;

		bool m_IsOnWindow;
		GPFramebuffer* m_Target;

		World* m_World;

		GPVertexShader* m_BasicVertexShader;
		GPVertexShader* m_GridVertexShader;

		GPPixelShader* m_ColorPixelShader;
		GPPixelShader* m_TexturePixelShader;

		GPConstantBuffer* m_ConstantBuffer;
		ConstantBuffer m_ConstantBufferData;
	};
}

extern Vortex::VXRenderer* GRenderer;
