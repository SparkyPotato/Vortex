project "Im3d"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	
	targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"im3d.h",
		"im3d_config.h",
		"im3d_math.h",
		"im3d.cpp"
	}
	
	optimize "on"
	symbols "off"
	
	filter "system:windows"
		systemversion "latest"
		
	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
		
	filter "configurations:DebugAll"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:DebugEngine"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:DebugApplication"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "on"
