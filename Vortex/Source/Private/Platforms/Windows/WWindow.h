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
		WWindow(const IWindow::Properties& properties);
		~WWindow();

	private:
		HWND m_WindowHandle;
		IWindow::Properties m_Properties;
	};
}

#endif
