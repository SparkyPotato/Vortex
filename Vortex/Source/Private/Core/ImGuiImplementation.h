#pragma once
#include <Core/IWindow.h>
#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/Windows/WWindow.h>
#include <Private/Platforms/DirectX11/DX11GraphicsContext.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_win32.cpp>
#include <examples/imgui_impl_dx11.h>
#include <examples/imgui_impl_dx11.cpp>
#endif

namespace Vortex
{
	class ImGuiImplementation
	{
	public:
		static void Init(IWindow* window)
		{
			#ifdef PLATFORM_WINDOWS
			ImGui_ImplWin32_Init(reinterpret_cast<WWindow*>(window)->GetWindowHandle());
			#endif

			if (IGraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(IGraphicsContext::Get());
				ImGui_ImplDX11_Init(context->GetDevice(), context->GetContext());
			}
		}

		static void NewFrame()
		{
			if (IGraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				ImGui_ImplDX11_NewFrame();
			}

			#ifdef PLATFORM_WINDOWS
			ImGui_ImplWin32_NewFrame();
			#endif
		}

		static void Draw(ImDrawData* data)
		{
			if (IGraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				ImGui_ImplDX11_RenderDrawData(data);
			}
		}

		static void Shutdown()
		{
			#ifdef PLATFORM_WINDOWS
			ImGui_ImplWin32_Shutdown();
			#endif

			if (IGraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				ImGui_ImplDX11_Shutdown();
			}
		}
	};
}
