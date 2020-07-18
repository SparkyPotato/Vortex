#include <VXpch.h>
#include <Core/VXCore.h>

namespace Vortex
{
	VXCore* VXCore::s_Core = nullptr;

	void VXCore::Startup()
	{
		if (s_Core)
		{
			ENG_ERROR("Cannot have two instances of Vortex Core!");
			return;
		}

		ENG_TRACE("Starting Vortex Core.");

		s_Core = new VXCore();

		ENG_TRACE("Started Vortex Core.");
	}

	int VXCore::Shutdown()
	{
		if (!s_Core)
		{
			ENG_ERROR("Vortex Core not started!");
			return -1;
		}

		ENG_TRACE("Shutting down Vortex Core.");

		delete s_Core;
		s_Core = nullptr;

		ENG_TRACE("Shutdown Vortex Core.");

		return 0;
	}

	VXCore::VXCore()
		: m_IsTicking(true)
	{

	}

	VXCore::~VXCore()
	{

	}

	void VXCore::RunCoreLoop()
	{
		while (m_IsTicking)
		{

		}
	}

	void VXCore::Quit()
	{
		m_TickMutex.lock();
		m_IsTicking = false;
		m_TickMutex.unlock();
	}
}
