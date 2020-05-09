project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	
	targetdir (raven_binprefix .. raven_outputdir .. "/%{prj.name}")
	objdir (raven_objprefix .. raven_outputdir .. "/%{prj.name}")

	files
	{
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/glfw_config.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c"
	}
	
	filter "system:windows"
		systemversion "latest"
		
		files
		{
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"
	
	targetdir (raven_binprefix .. raven_outputdir .. "/%{prj.name}")
	objdir (raven_objprefix .. raven_outputdir .. "/%{prj.name}")

	files
	{
		"glad/include/KHR/khrplatform.h",
		"glad/include/glad/glad.h",
		"glad/src/glad.c"
	}
	
	includedirs
	{
		"glad/include"
	}
	
	filter "system:windows"
		systemversion "latest"