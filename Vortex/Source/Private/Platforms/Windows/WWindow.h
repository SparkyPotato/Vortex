#pragma once

#ifdef PLATFORM_WINDOWS
#include <Core/IWindow.h>

namespace Vortex
{
	/*
		Windows Implementation of a IWindow.
	*/
	class WWindow : public IWindow
	{
	public:
		WWindow(const Properties& properties);
		~WWindow();

		virtual void SetEventCallback(EventCallback callback) { m_Callback = callback; }
		virtual void Update() override;

		static void RegisterWindowClass();

		static LRESULT WINAPI BaseWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI WindowThunk(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		LRESULT WINAPI WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		HWND m_WindowHandle;
		EventCallback m_Callback;
		Properties m_Properties;
	};
}

#endif
