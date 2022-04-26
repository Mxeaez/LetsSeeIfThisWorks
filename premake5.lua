workspace "LetsSeeIfThisWorks"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Submodules/GLFW/include"
IncludeDir["GLAD"] = "Submodules/GLAD/include"
IncludeDir["GLM"] = "Submodules/glm/glm"
IncludeDir["STBIMAGE"] = "Submodules/stb_image"

include "Submodules/GLFW"

project "LetsSeeIfThisWorks"
	location "LetsSeeIfThisWorks"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"Submodules/GLAD/src/glad.c",
		"Submodules/stb_image/stb_image.h",
		"%{prj.name}/src/Resources/**.png",
		"%{prj.name}/src/Shaders/**.glsl"
	}

	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STBIMAGE}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"