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
		ENG_TRACE("Creating window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		ENG_TRACE("Created window.");
	}

	WWindow::~WWindow()
	{
		ENG_TRACE("Destryoing window: \"{0}\" ({1}, {2}).", m_Properties.name, m_Properties.width, m_Properties.height);

		ENG_TRACE("Destroyed window.");
	}
}

#endif 
