#include <VXpch.h>
#include <Core/Profiler.h>

namespace Vortex
{
	LARGE_INTEGER Profiler::m_Frequency;
	std::map<std::string, float> Profiler::m_LastTimes;

	Profiler::Profiler(std::string name, Location loc)
	{
		m_Name = name;
		m_Loc = loc;

		QueryPerformanceCounter(&m_StartTime);
	}

	Profiler::~Profiler()
	{
		QueryPerformanceCounter(&m_StopTime);

		float time = (float) (m_StopTime.QuadPart - m_StartTime.QuadPart);
		time /= (float) m_Frequency.QuadPart;
		m_LastTimes[m_Name] = time;
	}

	void Profiler::Init()
	{
		QueryPerformanceFrequency(&m_Frequency);
		m_Frequency.QuadPart /= 1000;
	}

	void Profiler::Shutdown()
	{

	}
}
