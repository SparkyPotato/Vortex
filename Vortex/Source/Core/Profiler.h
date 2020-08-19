#pragma once
#include <map>
#ifdef PLATFORM_WINDOWS
#include <winnt.h>
#endif

namespace Vortex
{
	/*
		Profiler to measure time taken by a function.
		Has an accuracy of around 1 microsecond.
		Is currently platform-specific.
	*/
	class Profiler
	{
	public:
		/*
			Who is doing the profiling?
		*/
		enum class Location
		{
			Engine, Client
		};

		Profiler(std::string name, Location loc);
		~Profiler();

		/*
			Initializes the profiler.
		*/
		static void Init();
		static void Shutdown();

		/*
			Gets all the recorded profiles.
		*/
		static const std::map<std::string, float>& GetProfiles() { return m_LastTimes; }

	private:
		static LARGE_INTEGER m_Frequency;
		static std::map<std::string, float> m_LastTimes;

		// Specific for each profiler object.
		std::string m_Name;
		Location m_Loc;
		LARGE_INTEGER m_StartTime;
		LARGE_INTEGER m_StopTime;
	};
}
/*
	Profile a specific scope. Profiling ends when out of the scope.
*/
#define ENG_PROFILE(name) auto profilerObject = Vortex::Profiler(name, Vortex::Profiler::Location::Engine)
/*
	Profiles a set of statements. Use the scope-profile whenever possible.
*/
#define ENG_PROFILESTART(name) auto profilerObject = new Vortex::Profiler(name, Vortex::Profiler::Location::Engine)
#define ENG_PROFILEEND() delete profilerObject

/*
	Same as the engine profiles, with the client location instead.
*/
#define VX_PROFILE(name) auto profilerObject = Vortex::Profiler(name, Vortex::Profiler::Location::Client)
