workspace "Mashenka"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory) --
IncludeDir = {}
IncludeDir["GLFW"] = "Mashenka/vendor/GLFW/include"
IncludeDir["Glad"] = "Mashenka/vendor/Glad/include"
IncludeDir["ImGui"] = "Mashenka/vendor/imgui"

-- Include the premake config for GLFW from the submodule, which contains how to build it --
include "Mashenka/vendor/GLFW"
include "Mashenka/vendor/Glad"
include "Mashenka/vendor/imgui"

project "Mashenka"
    location "Mashenka"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    pchheader "mkpch.h"
    pchsource "Mashenka/src/mkpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
    }

-- Links the GLFW and opengl lib --
    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
        "ImGui"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime = "On"
        systemversion "latest"

        defines
        {
            "MK_PLATFORM_WINDOWS",
            "MK_BUILD_DLL",
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "MK_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs
    {
        "Mashenka/vendor/spdlog/include",
        "Mashenka/src"
    }

    links
    {
        "Mashenka"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime = "On"
        systemversion "latest"

        defines
        {
            "MK_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "MK_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"
        buildoptions "/MD"
        optimize "On"