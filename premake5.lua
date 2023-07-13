workspace "Mashenka"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Mashenka"
    location "Mashenka"
    kind "SharedLib"
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
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime = "On"
        systemversion "10.0"

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
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"         
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
        systemversion "10.0"

        defines
        {
            "MK_PLATFORM_WINDOWS",
        }
    
    filter "configurations:Debug"
        defines "MK_DEBUG"
        symbols "On"

    filter "configurations:Release"             
        defines "MK_RELEASE"
        optimize "On"

    filter "configurations:Dist"  
        defines "MK_DIST"         
        optimize "On"