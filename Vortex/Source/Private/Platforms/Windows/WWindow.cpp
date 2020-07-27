#include <VXpch.h>

#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/Windows/WWindow.h>
#include <Core/Events/WindowEvent.h>
#include <Core/Events/InputEvent.h>
#include <Graphics/Primitives/GPFramebuffer.h>

namespace Vortex
{
	IWindow* IWindow::Create(const IWindow::Properties& properties)
	{
		// Create the WWindow.
		return new WWindow(properties);
	}

	WWindow::WWindow(const IWindow::Properties& properties)
		: m_Properties(properties)
	{
		ENG_TRACE("Creating window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		// Adjust size for window style.
		RECT windowRect;
		windowRect.top = 100;
		windowRect.bottom = 100 + m_Properties.height;
		windowRect.left = 100;
		windowRect.right = 100 + m_Properties.width;
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		// Convert name to Unicode.
		std::string temp = m_Properties.name;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring name(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &name[0], size_needed);
		// Create window.
		m_WindowHandle = CreateWindow
		(
			L"Vortex Window",
			name.c_str(),
			WS_OVERLAPPEDWINDOW,
			m_Properties.x, m_Properties.y,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			this
		);

		if (m_WindowHandle == NULL)
			throw std::exception("Failed to create window.");

		// Show window on the screen.
		ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);

		// Create the swap chain.
		m_SwapChain = GPSwapChain::Create(this);

		ENG_TRACE("Created window.");
	}

	WWindow::~WWindow()
	{
		ENG_TRACE("Destroying window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		if (m_Framebuffer) m_Framebuffer->SetWindow(nullptr);
		delete m_SwapChain;

		// Remove the window from the screen.
		DestroyWindow(m_WindowHandle);

		ENG_TRACE("Destroyed window.");
	}

	void WWindow::Update()
	{
		// Show the last frame.
		m_SwapChain->Swap(m_Properties.syncInterval);

		// Message loop.
		MSG message;
		while (PeekMessage(&message, m_WindowHandle, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	void WWindow::Activate()
	{
		SetActiveWindow(m_WindowHandle);
	}

	void WWindow::SetName(std::string name)
	{
		// Convert name to Unicode.
		m_Properties.name = name;
		std::string temp = m_Properties.name;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), NULL, 0);
		std::wstring wname (size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, temp.c_str(), (int)temp.size(), &wname[0], size_needed);

		SetWindowText(m_WindowHandle, wname.c_str());
	}

	void WWindow::SetSize(int width, int height)
	{
		m_Properties.width = width;
		m_Properties.height = height;

		// Adjust window size based on style.
		RECT windowRect;
		windowRect.top = 100;
		windowRect.bottom = 100 + m_Properties.height;
		windowRect.left = 100;
		windowRect.right = 100 + m_Properties.width;
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		SetWindowPos(m_WindowHandle, HWND_TOP, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE);
	}

	void WWindow::SetPosition(int x, int y)
	{
		m_Properties.x = x;
		m_Properties.y = y;
		SetWindowPos(m_WindowHandle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);
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

		if (RegisterClassEx(&windowClass) == 0)
			throw std::exception("Failed to register window class");

		ENG_TRACE("Registered window class.");
	}

	LRESULT WWindow::BaseWindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Sets up thunking to the correct window.
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
		// Call the window-specific message handler.
		WWindow* p_Window = reinterpret_cast<WWindow*>(GetWindowLongPtr(window, GWLP_USERDATA));
		return p_Window->WindowProc(window, message, wParam, lParam);
	}

	LRESULT WWindow::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Message handler
		switch (message)
		{
		case WM_CLOSE:
		{
			if (m_Callback) m_Callback(this, WindowCloseEvent());
			return 0;
		}
		case WM_SIZE:
		{
			// Resize the swap chain.
			if (m_Framebuffer) m_Framebuffer->Resize();
			else if (m_SwapChain) m_SwapChain->Resize();
			m_Properties.width = LOWORD(lParam);
			m_Properties.height = HIWORD(lParam);

			if (wParam == SIZE_MAXIMIZED)
			{
				ENG_TRACE("Window \"{0}\" maximized.", m_Properties.name);
				m_Properties.isMinimized = false;
				m_Properties.isMaximized = true;
				if (m_Callback) m_Callback(this, WindowMaximizeEvent());

				ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
				if (m_Callback) m_Callback(this, WindowResizeEvent(m_Properties.width, m_Properties.height));
				m_Properties.isResizing = false;
			}
			else if (wParam == SIZE_MINIMIZED)
			{
				ENG_TRACE("Window \"{0}\" minimized.", m_Properties.name)
				m_Properties.isMinimized = true;
				m_Properties.isMaximized = false;
				if (m_Callback) m_Callback(this, WindowMinimizeEvent());

				ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
				if (m_Callback) m_Callback(this, WindowResizeEvent(m_Properties.width, m_Properties.height));
				m_Properties.isResizing = false;
			}
			else if (wParam == SIZE_RESTORED)
			{
				m_Properties.isResizing = true;
				if (m_Properties.isMaximized)
				{
					ENG_TRACE("Window \"{0}\" unmaximized.", m_Properties.name);
					m_Properties.isMaximized = false;
					if (m_Callback) m_Callback(this, WindowUnmaximizeEvent());

					ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
					if (m_Callback) m_Callback(this, WindowResizeEvent(m_Properties.width, m_Properties.height));
					m_Properties.isResizing = false;
				}
				else if (m_Properties.isMinimized)
				{
					ENG_TRACE("Window \"{0}\" unminimized", m_Properties.name);
					m_Properties.isMinimized = false;
					if (m_Callback) m_Callback(this, WindowUnminimizeEvent());

					ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
					if (m_Callback) m_Callback(this, WindowResizeEvent(m_Properties.width, m_Properties.height));
					m_Properties.isResizing = false;
				}
			}
			else
				m_Properties.isMinimized = false;
			break;
		}
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);

			RECT windowRect;
			windowRect.top = 100;
			windowRect.bottom = 100 + m_Properties.minHeight;
			windowRect.left = 100;
			windowRect.right = 100 + m_Properties.minWidth;
			AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

			info->ptMinTrackSize = { windowRect.right - windowRect.left, windowRect.bottom - windowRect.top };
			break;
		}
		case WM_MOVE:
		{
			m_Properties.isMoving = true;
			POINTS points = MAKEPOINTS(lParam);
			m_Properties.x = points.x;
			m_Properties.y = points.y;
			break;
		}
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) != WA_INACTIVE)
			{
				m_Properties.isActive = true;
				ENG_TRACE("Window \"{0}\" activated.", m_Properties.name);
				if (m_Callback) m_Callback(this, WindowActivateEvent());
			}
			else
			{
				m_Properties.isActive = false;
				ENG_TRACE("Window \"{0}\" deactivated.", m_Properties.name);
				if (m_Callback) m_Callback(this, WindowDeactivateEvent());
			}
			break;
		}
		case WM_ENTERSIZEMOVE:
		{
			break;
		}
		case WM_EXITSIZEMOVE:
		{
			if (m_Properties.isResizing)
			{
				ENG_TRACE("Window \"{0}\" resized to ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);
				if (m_Callback) m_Callback(this, WindowResizeEvent(m_Properties.width, m_Properties.height));
				m_Properties.isResizing = false;
			}

			if (m_Properties.isMoving)
			{
				ENG_TRACE("Window \"{0}\" moved to ({1}, {2}).", m_Properties.name, m_Properties.x, m_Properties.y);
				if (m_Callback) m_Callback(this, WindowMoveEvent(m_Properties.x, m_Properties.y));
				m_Properties.isMoving = false;
			}
			break;
		}
		case WM_KEYDOWN:
		{
			if (m_Callback) m_Callback(this, KeyDownEvent((int) wParam));
			break;
		}
		case WM_KEYUP:
		{
			if (m_Callback) m_Callback(this, KeyUpEvent((int) wParam));
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS points = MAKEPOINTS(lParam);
			if (m_Callback) m_Callback(this, MouseMoveEvent(points.x, points.y));
		}
		case WM_LBUTTONDOWN:
		{
			if (m_Callback) m_Callback(this, MouseButtonDownEvent((int) InputCode::LeftButton));
		}
		case WM_LBUTTONUP:
		{
			if (m_Callback) m_Callback(this, MouseButtonUpEvent((int)InputCode::LeftButton));
		}
		case WM_LBUTTONDBLCLK:
		{
			if (m_Callback) m_Callback(this, MouseButtonDoubleClickEvent((int)InputCode::LeftButton));
		}
		case WM_RBUTTONDOWN:
		{
			if (m_Callback) m_Callback(this, MouseButtonDownEvent((int)InputCode::RightButton));
		}
		case WM_RBUTTONUP:
		{
			if (m_Callback) m_Callback(this, MouseButtonUpEvent((int)InputCode::RightButton));
		}
		case WM_RBUTTONDBLCLK:
		{
			if (m_Callback) m_Callback(this, MouseButtonDoubleClickEvent((int)InputCode::RightButton));
		}
		case WM_MBUTTONDOWN:
		{
			if (m_Callback) m_Callback(this, MouseButtonDownEvent((int)InputCode::MiddleButton));
		}
		case WM_MBUTTONUP:
		{
			if (m_Callback) m_Callback(this, MouseButtonUpEvent((int)InputCode::MiddleButton));
		}
		case WM_MBUTTONDBLCLK:
		{
			if (m_Callback) m_Callback(this, MouseButtonDoubleClickEvent((int)InputCode::MiddleButton));
		}
		case WM_MOUSEWHEEL:
		{
			if (m_Callback) m_Callback(this, MouseScrollEvent((float) GET_WHEEL_DELTA_WPARAM(wParam) / (float) WHEEL_DELTA));
		}
		}
		return DefWindowProc(window, message, wParam, lParam);
	}
}

#endif 
