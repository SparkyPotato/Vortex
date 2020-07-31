#pragma once
#include <string>
#include <map>
#ifdef PLATFORM_WINDOWS
#include <winnt.h>
#endif

namespace Vortex
{
	class Profiler
	{
	public:
		enum class Location
		{
			Engine, Client
		};

		Profiler(std::string name, Location loc);
		~Profiler();

		static void Init();
		static void Shutdown();

		static const std::map<std::string, float>& GetProfiles() { return m_LastTimes; }

	private:
		static LARGE_INTEGER m_Frequency;
		static std::map<std::string, float> m_LastTimes;

		std::string m_Name;
		Location m_Loc;
		LARGE_INTEGER m_StartTime;
		LARGE_INTEGER m_StopTime;
	};
}

#define ENG_PROFILE(name) auto p = Vortex::Profiler(name, Vortex::Profiler::Location::Engine)
#define ENG_PROFILESTART(name) auto p = new Vortex::Profiler(name, Vortex::Profiler::Location::Engine)
#define ENG_PROFILEEND() delete p

#define VX_PROFILE(name) auto p = Vortex::Profiler(name, Vortex::Profiler::Location::Client)