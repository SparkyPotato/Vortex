#include <VXpch.h>
#include <Core/VXModules/VXCore.h>

namespace Vortex
{
	int VXCore::Startup()
	{
		ENG_TRACE("Starting Vortex Core Module.");

		if (m_IsTicking)
		{
			ENG_ERROR("Vortex Core Module is already started!");
			return -1;
		}

		m_IsTicking = true;
		QueryPerformanceFrequency(&m_Frequency);
		m_DeltaTime = 0;

		ENG_TRACE("Started Vortex Core Module.");
		return 0;
	}

	int VXCore::Shutdown()
	{
		ENG_TRACE("Shutting down Vortex Core Module.");

		

		ENG_TRACE("Shut down Vortex Core Module.");
		return 0;
	}

	void VXCore::Tick(float deltaTime)
	{
		
	}

	void VXCore::RunTickLoop()
	{
		while (m_IsTicking)
		{
			QueryPerformanceCounter(&m_LastTime);

			Tick(m_DeltaTime);

			QueryPerformanceCounter(&m_CurrentTime);
			m_DeltaTime = m_CurrentTime.QuadPart - m_LastTime.QuadPart;
			m_DeltaTime /= m_Frequency.QuadPart;
		}
	}

	void VXCore::Quit()
	{
		m_RunMutex.lock();
		m_IsTicking = false;
		m_RunMutex.unlock();
	}

	VXCore::VXCore()
	{
		m_IsTicking = false;
	}

	VXCore::~VXCore()
	{

	}
}
