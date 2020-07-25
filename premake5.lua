workspace "Vortex"
	architecture "x86_64"
	startproject "VortexEditor"
	
	configurations
	{
		"DebugAll",
		"DebugEngine",
		"DebugApplication",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
outputdir = "%{cfg.buildcfg}/"

project "Vortex"
	location "Vortex"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	pchheader "VXpch.h"
	pchsource "Vortex/Source/VXpch.cpp"
	
	characterset "Unicode"
	
	targetdir ("Binaries/" .. outputdir .. "%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "%{prj.name}")
	
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	
	includedirs
	{
		"Vortex/Source",
		"Vortex/ThirdParty/spdlog/include"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"
		
	filter "configurations:DebugAll"
		defines "CFG_DEBUGALL"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:DebugEngine"
		defines "CFG_DEBUGENG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:DebugApplication"
		defines "CFG_DEBUGAPP"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Release"
		defines "CFG_RELEASE"
		runtime "Release"
		optimize "on"
		
project "VortexEditor"
	location "VortexEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	characterset "Unicode"
	
	targetdir ("Binaries/" .. outputdir .. "%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "%{prj.name}")
	
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	
	includedirs
	{
		"Vortex/Source",
		"Vortex/ThirdParty/spdlog/include"
	}
	
	links 
	{
		"Vortex"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines "PLATFORM_WINDOWS"
		entrypoint "wmainCRTStartup"
		
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