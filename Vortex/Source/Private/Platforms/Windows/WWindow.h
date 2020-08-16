#pragma once

#ifdef PLATFORM_WINDOWS
#include <Core/Window.h>

namespace Vortex
{
	/*
		Windows Implementation of IWindow.
	*/
	class WWindow : public Window
	{
	public:
		WWindow(const Properties& properties);
		virtual ~WWindow();

		virtual void SetEventCallback(EventCallback callback) { m_Callback = callback; }
		virtual void Update() override;

		virtual const Properties& GetProperties() override { return m_Properties; }

		virtual void Activate() override;
		virtual void SetName(std::string name) override;
		virtual void SetSize(int width, int height) override;
		virtual void SetPosition(int x, int y) override;

		virtual GPSwapChain* GetSwapChain() override { return m_SwapChain; }
		virtual GPFramebuffer* GetFramebuffer() override { return m_Framebuffer; }

		virtual void OnConsoleCommand(ConsoleCommand command) override;

		/*
			Registers window class, called in the Windows Entry Point.
		*/
		static void RegisterWindowClass();

		/*
			Returns the native window handle to do stuff with.
		*/
		HWND GetWindowHandle() { return m_WindowHandle; }

		/*
			Windows message handlers.
		*/
		static LRESULT WINAPI BaseWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI WindowThunk(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		LRESULT WINAPI WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_WindowHandle;
		EventCallback m_Callback;
		Properties m_Properties;

		GPSwapChain* m_SwapChain = nullptr;
		GPFramebuffer* m_Framebuffer = nullptr;
	};
}

#endif
