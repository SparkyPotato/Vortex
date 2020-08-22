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

Includes = {}
Includes["spdlog"] = "Vortex/ThirdParty/spdlog/include"
Includes["imgui"] = "Vortex/ThirdParty/imgui"
Includes["im3d"] = "Vortex/ThirdParty/im3d"

include "Vortex/ThirdParty/imgui"
include "Vortex/ThirdParty/im3d"

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
	
	links
	{
		"imgui",
		"im3d"
	}
	
	files
	{
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp"
	}
	
	includedirs
	{
		"Vortex/Source",
		"%{Includes.spdlog}",
		"%{Includes.imgui}",
		"%{Includes.im3d}"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:DebugAll"
		defines 
		{ 
			"CFG_DEBUGENG",
			"CFG_DEBUGAPP"
		}
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
	kind "WindowedApp"
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
		"VortexEditor/Source",
		"Vortex/Source",
		"%{Includes.spdlog}",
		"%{Includes.imgui}",
		"%{Includes.im3d}"
	}
	
	links 
	{
		"Vortex",
		"imgui"
	}
	
	filter "system:windows"
		systemversion "latest"
		entrypoint "wmainCRTStartup"
		
	filter "configurations:DebugAll"
		defines 
		{ 
			"CFG_DEBUGENG",
			"CFG_DEBUGAPP"
		}
		
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