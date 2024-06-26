workspace "Mashenka"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile" -- enable multi-processor compilation --
    
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory) --
IncludeDir = {}
IncludeDir["GLFW"] = "Mashenka/vendor/GLFW/include"
IncludeDir["Glad"] = "Mashenka/vendor/Glad/include"
IncludeDir["ImGui"] = "Mashenka/vendor/imgui"
IncludeDir["glm"] = "Mashenka/vendor/glm"
IncludeDir["stb_image"] = "Mashenka/vendor/stb_image"

-- Include the premake config for GLFW from the submodule, which contains how to build it --
-- Group Dependencies
group "Dependencies"
    include "Mashenka/vendor/GLFW"
    include "Mashenka/vendor/Glad"
    include "Mashenka/vendor/imgui"

group ""


project "Mashenka"
    location "Mashenka"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" -- using a dynamic runtime, using extra DLL, thus changes will be reflected when building sandbox

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
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }
    
    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

-- Links the GLFW and opengl lib --
    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
        "ImGui"
    }

    -- define the pre-processer definitions --
    filter "system:windows"
        systemversion "latest"

        defines
        {

        }

-- using \" to enclose the argument in case special characters in the path --

    
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
    staticruntime "on"
    cppdialect "C++17"

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
        systemversion "latest"

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
        

project "Mashenka-Editor"
    location "Mashenka-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%/{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Mashenka/vendor/spdlog/include",
        "Mashenka/src",
        "Mashenka/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Mashenka"
    }

    filter "system:windows"
        systemversion "latest"

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