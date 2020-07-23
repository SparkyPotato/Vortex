#include <VXpch.h>

#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/Windows/WWindow.h>
#include <Core/Events/WindowEvent.h>

namespace Vortex
{
	IWindow* IWindow::Create(const IWindow::Properties& properties)
	{
		return new WWindow(properties);
	}

	WWindow::WWindow(const IWindow::Properties& properties)
		: m_Properties(properties)
	{
		ENG_TRACE("Creating window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		std::string temp = m_Properties.name;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring name(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &name[0], size_needed);
		m_WindowHandle = CreateWindow
		(
			L"Vortex Window",
			name.c_str(),
			WS_OVERLAPPEDWINDOW,
			m_Properties.x, m_Properties.y,
			m_Properties.width, m_Properties.height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this
		);

		ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);

		ENG_TRACE("Created window.");
	}

	WWindow::~WWindow()
	{
		ENG_TRACE("Destroying window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		DestroyWindow(m_WindowHandle);

		ENG_TRACE("Destroyed window.");
	}

	void WWindow::Update()
	{
		MSG message;
		while (PeekMessage(&message, m_WindowHandle, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void WWindow::RegisterWindowClass()
	{
		WNDCLASSEX windowClass = { 0 };
		ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = BaseWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hIconSm = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = L"Vortex Window";

		RegisterClassEx(&windowClass);

		ENG_TRACE("Registered window class.");
	}

	LRESULT WWindow::BaseWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			WWindow* p_Window = reinterpret_cast<WWindow*>(createStruct->lpCreateParams);

			SetWindowLongPtr(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_Window));
			SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowThunk));

			return p_Window->WindowProc(window, message, wParam, lParam);
		}
		return DefWindowProc(window, message, wParam, lParam);
	}

	LRESULT WWindow::WindowThunk(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WWindow* p_Window = reinterpret_cast<WWindow*>(GetWindowLongPtr(window, GWLP_USERDATA));
		return p_Window->WindowProc(window, message, wParam, lParam);
	}

	LRESULT WWindow::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
		{
			if (m_Callback) m_Callback(this, WindowCloseEvent());
			return 0;
		}
		case WM_SIZE:
		{
			m_Properties.width = LOWORD(lParam);
			m_Properties.height = HIWORD(lParam);
			ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
			if (m_Callback) m_Callback(this, WindowResizeEvent(LOWORD(lParam), HIWORD(lParam)));

			if (wParam == SIZE_MAXIMIZED)
			{
				ENG_TRACE("Window \"{0}\" maximized.", m_Properties.name);
				m_Properties.IsMinimized = false;
				m_Properties.IsMaximized = true;
				if (m_Callback) m_Callback(this, WindowMaximizeEvent());
			}
			else if (wParam == SIZE_MINIMIZED)
			{
				ENG_TRACE("Window \"{0}\" minimized.", m_Properties.name)
				m_Properties.IsMinimized = true;
				m_Properties.IsMaximized = false;
				if (m_Callback) m_Callback(this, WindowMinimizeEvent());
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_Properties.IsMaximized)
				{
					ENG_TRACE("Window \"{0}\" unmaximized.", m_Properties.name);
					m_Properties.IsMaximized = false;
					if (m_Callback) m_Callback(this, WindowUnmaximizeEvent());
				}
				else if (m_Properties.IsMinimized)
				{
					ENG_TRACE("Window \"{0}\" unminimized", m_Properties.name);
					m_Properties.IsMinimized = false;
					if (m_Callback) m_Callback(this, WindowUnminimizeEvent());
				}
			}
			else
				m_Properties.IsMinimized = false;
			break;
		}
		case WM_MOVE:
		{
			m_Properties.x = LOWORD(lParam);
			m_Properties.y = HIWORD(lParam);
			ENG_TRACE("Window \"{0}\" moved to ({1}, {2}).", m_Properties.name, m_Properties.x, m_Properties.y);
			if (m_Callback) m_Callback(this, WindowMoveEvent(LOWORD(lParam), HIWORD(lParam)));
			break;
		}
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) != WA_INACTIVE)
			{
				m_Properties.IsActive = true;
				ENG_TRACE("Window \"{0}\" activated.", m_Properties.name);
				if (m_Callback) m_Callback(this, WindowActivateEvent());
			}
			else
			{
				m_Properties.IsActive = false;
				ENG_TRACE("Window \"{0}\" deactivated", m_Properties.name);
				if (m_Callback) m_Callback(this, WindowDeactivateEvent());
			}
		}
		}
		return DefWindowProc(window, message, wParam, lParam);
	}
}

#endif 
