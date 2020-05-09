-- Values to be used in your project to ensure compatibility.
raven_language = "C++"
raven_cppdialect = "C++17"


-- Additional include directories, that need to be included in your project.
-- Example: includedirs { raven_include_directories, ... }
raven_include_directories = {}


raven_internal_include_directories = {}
raven_internal_include_directories["Raven"] = "Raven/src"
raven_internal_include_directories["Raven_Core"] = "Raven_Core/src"
raven_internal_include_directories["spdlog"] = "vendor/spdlog/include"
raven_internal_include_directories["GLFW"] = "vendor/glfw/include"


for key,value in pairs(raven_internal_include_directories) do
    raven_include_directories[key] = path_to_raven .. raven_internal_include_directories[key]
end

-- Architecture, Raven only supports only 64-bit. This probably won't change.
-- 32-bit might work but this isn't guaranteed.
architecture "x86_64"

-- Configurations
configurations
{
	"Debug", -- Has logging and debug symbols
	"Release", -- Has logging and optimization
	"Distribution" -- No logging and optimization, build to be distributed
}

-- Flags
flags
{
	"MultiProcessorCompile"
}

-- Paths for build target
raven_outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
raven_objprefix = "bin-int/"
raven_binprefix = "bin/"

-- Set groups
group "Engine/Dependencies"
include "vendor"

group "Engine"

-- Project: Raven
-- This merges all of the subprojects into one project, which your projects need to link to
project "Raven"
	location "Raven"
	kind "StaticLib"
	language (raven_language)
	cppdialect (raven_cppdialect)
	staticruntime "on"
	
	-- The build paths might be changeable in the future
	objdir (raven_objprefix .. raven_outputdir .. "/%{prj.name}")
	targetdir (raven_binprefix .. raven_outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "Raven/src/pch.cpp"
	
	-- All headers and source files are included
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		raven_internal_include_directories
	}
	
	links
	{
		"Raven_Core",
		"GLFW"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "DIST"
		optimize "on"
		runtime "Release"

-- Project: Raven_Core
-- This project implements core features that all other parts of Raven might need
project "Raven_Core"
	location "Raven_Core"
	kind "StaticLib"
	language (raven_language)
	cppdialect (raven_cppdialect)
	staticruntime "on"
	
	-- The build paths might be changeable in the future
	objdir (raven_objprefix .. raven_outputdir .. "/%{prj.name}")
	targetdir (raven_binprefix .. raven_outputdir .. "/%{prj.name}")
	
	-- All headers and source files are included
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{raven_internal_include_directories.Raven_Core}",
		"%{raven_internal_include_directories.spdlog}"
	}
	
	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "DIST"
		optimize "on"
		runtime "Release"
	
group "Projects"