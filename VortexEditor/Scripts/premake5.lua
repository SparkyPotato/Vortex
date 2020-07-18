newoption
{
	trigger = "projectName",
	value = "name",
	description = "Name of the Project"
}

newoption
{
	trigger = "enginePath",
	value = "path",
	description = "Path of the installed Engine"
}

workspace (_OPTIONS["projectName"])
	architecture "x86_64"
	startproject (_OPTIONS["projectName"])
	
	configurations
	{
		"DebugAll",
		"DebugEngine",
		"DebugApp",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.system}/%{cfg.buildcfg}/"
		
project (_OPTIONS["projectName"])
	location "Source"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("Bin/" .. outputdir .. "%{prj.name}")
	objdir ("Bin/Int/" .. outputdir .. "%{prj.name}")
	
	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}
	
	includedirs
	{
		(_OPTIONS["enginePath"] .. "/Vortex/Source")
	}
	
	links 
	{
		(_OPTIONS["enginePath"] .. "/Bin/" .. outputdir .. "/Vortex/Vortex.lib")
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:DebugAll"
		defines "CFG_DEBUGALL"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:DebugEngine"
		defines "CFG_DEBUGENG"
		runtime "Release"
		optimize "on"
		
	filter "configurations:DebugApplication"
		defines "CFG_DEBUGAPP"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		runtime "Release"
		optimize "on"