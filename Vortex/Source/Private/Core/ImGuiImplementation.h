#pragma once
#include <Core/Window.h>
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
	/*
		The implementation of ImGui, since it varies per-platform.
	*/
	class ImGuiImplementation
	{
	public:
		/*
			Initializes the ImGui implementation.
		*/
		static void Init(Window* window)
		{
			#ifdef PLATFORM_WINDOWS
			ImGui_ImplWin32_Init(reinterpret_cast<WWindow*>(window)->GetWindowHandle());

			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				DX11GraphicsContext* context = reinterpret_cast<DX11GraphicsContext*>(GraphicsContext::Get());
				ImGui_ImplDX11_Init(context->GetDevice(), context->GetContext());
			}
			#endif
		}

		/*
			Begins a new frame.
		*/
		static void NewFrame()
		{
			#ifdef PLATFORM_WINDOWS
			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				ImGui_ImplDX11_NewFrame();
			}

			ImGui_ImplWin32_NewFrame();
			#endif
		}

		/*
			Draws the data.
		*/
		static void Draw(ImDrawData* data)
		{
			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				GraphicsContext::Get()->Lock();

				ImGui_ImplDX11_RenderDrawData(data);

				GraphicsContext::Get()->Unlock();
			}
		}

		/*
			Shuts the implementation down.
		*/
		static void Shutdown()
		{
			#ifdef PLATFORM_WINDOWS
			ImGui_ImplWin32_Shutdown();
			#endif

			if (GraphicsContext::Get()->GetAPI() == GraphicsAPI::DirectX11)
			{
				ImGui_ImplDX11_Shutdown();
			}
		}
	};
}
