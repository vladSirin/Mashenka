workspace "Mashenka"
    architecture "x64"
    startproject "Sandbox"

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
IncludeDir["glm"] = "Mashenka/vendor/glm"

-- Include the premake config for GLFW from the submodule, which contains how to build it --
-- Group Dependencies
group "Dependencies"
    include "Mashenka/vendor/GLFW"
    include "Mashenka/vendor/Glad"
    include "Mashenka/vendor/imgui"

group ""


project "Mashenka"
    location "Mashenka"
    kind "SharedLib"
    language "C++"
    staticruntime "off" -- using a dynamic runtime, using extra DLL, thus changes will be reflected when building sandbox

    targetdir ("bin/" ..outputdir.. "/%{prj.name}")
    objdir ("bin-int/" ..outputdir.. "/%{prj.name}")

    pchheader "mkpch.h"
    pchsource "Mashenka/src/mkpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }
    
    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
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
        systemversion "latest"

        defines
        {
            "MK_PLATFORM_WINDOWS",
            "MK_BUILD_DLL",
        }

-- using \" to enclose the argument in case special characters in the path --
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir.. "/Sandbox/\"")
        }
    
    filter "configurations:Debug"
        defines "MK_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "off"

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
        "Mashenka/src",
        "%{IncludeDir.glm}",
        "Mashenka/vendor"
    }

    links
    {
        "Mashenka"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "MK_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "MK_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"
        runtime "Release"
        optimize "On"