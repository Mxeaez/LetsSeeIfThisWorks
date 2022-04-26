workspace "LetsSeeIfThisWorks"
	architecture "x64"
	configurations { "Debug", "Release" }
	staticruntime "off"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Submodules/GLFW/include"
IncludeDir["GLAD"] = "Submodules/GLAD/include"
IncludeDir["GLM"] = "Submodules/glm/glm"
IncludeDir["STBIMAGE"] = "Submodules/stb_image"
IncludeDir["SPDLOG"] = "Submodules/spdlog/include"
IncludeDir["IMGUI"] = "Submodules/imgui"

include "Submodules/GLFW"
include "Submodules/GLAD"
include "Submodules/imgui"
include "Submodules/spdlog"

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
		"%{prj.name}/src/Resources/**.png",
		"%{prj.name}/src/Shaders/**.glsl",
		"Submodules/stb_image/stb_image.h"
	}

	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STBIMAGE}",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.IMGUI}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"spdlog",
		"opengl32.lib"
	}

	filter "configurations:Debug"
		defines { "DEBUG" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"