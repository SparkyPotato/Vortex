#include <VXpch.h>

#ifdef PLATFORM_WINDOWS
#include <Private/Platforms/Windows/WWindow.h>

namespace Vortex
{
	IWindow* InstantiateWindow(const IWindow::Properties& properties)
	{
		return new WWindow(properties);
	}

	WWindow::WWindow(const IWindow::Properties& properties)
		: m_Properties(properties)
	{

	}

	WWindow::~WWindow()
	{

	}
}

#endif 
