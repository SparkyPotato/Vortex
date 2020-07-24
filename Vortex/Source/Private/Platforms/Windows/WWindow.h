#pragma once

#ifdef PLATFORM_WINDOWS
#include <Core/IWindow.h>

namespace Vortex
{
	/*
		Windows Implementation of IWindow.
	*/
	class WWindow : public IWindow
	{
	public:
		WWindow(const Properties& properties);
		~WWindow();

		virtual void SetEventCallback(EventCallback callback) { m_Callback = callback; }
		virtual void Update() override;

		virtual void Activate() override;
		virtual void SetName(std::string name) override;
		virtual void SetSize(int width, int height) override;
		virtual void SetPosition(int x, int y) override;

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
