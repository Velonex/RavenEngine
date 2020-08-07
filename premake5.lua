-- Example project file for a Raven project
workspace "Raven"
	startproject "Sandbox"
	path_to_raven = "Raven/" -- This needs to be set so to make the include directories point to the correct place
	include "Raven" -- This path needs to be changed to fit the path to the Raven subdirectory

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	-- The language and the C++ dialect are set using these variables to guarantee compatibility with Raven
	language (raven_language)
	cppdialect (raven_cppdialect)
	staticruntime "on"
	
	-- This redefines the output paths but isn't necessary
	objdir (raven_objprefix .. raven_outputdir .. "/%{prj.name}")
	targetdir (raven_binprefix .. raven_outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		raven_include_directories,
		"%{prj.name}/src"
	}
	
	links
	{
		"Raven"
	}
	
	filter "system:windows"
		systemversion "latest"
	
	-- This are the recommended configurations
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Distribution"
		defines "DIST"
		optimize "on"
		runtime "Release"