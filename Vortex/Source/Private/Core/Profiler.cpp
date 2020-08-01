#include <VXpch.h>
#include <Core/Profiler.h>

namespace Vortex
{
	LARGE_INTEGER Profiler::m_Frequency;
	std::map<std::string, float> Profiler::m_LastTimes;

	Profiler::Profiler(std::string name, Location loc)
	{
		// Set the name and location.
		m_Name = name;
		m_Loc = loc;

		// Get the start-time.
		QueryPerformanceCounter(&m_StartTime);
	}

	Profiler::~Profiler()
	{
		// Get end time.
		QueryPerformanceCounter(&m_StopTime);

		// Calculate and push into the profiles vector.
		float time = (float) (m_StopTime.QuadPart - m_StartTime.QuadPart);
		time /= (float) m_Frequency.QuadPart;
		m_LastTimes[m_Name] = time;
	}

	void Profiler::Init()
	{
		// Get the frequency of the perfomance-counter.
		QueryPerformanceFrequency(&m_Frequency);
		m_Frequency.QuadPart /= 1000;
	}

	void Profiler::Shutdown()
	{

	}
}
