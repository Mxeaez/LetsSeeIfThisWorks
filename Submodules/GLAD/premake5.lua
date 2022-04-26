project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("${solutiondir}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("${solutiondir}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c",
	}

	includedirs
    {
        "include"
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"