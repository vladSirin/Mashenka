project "Mashenka"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on" -- using a dynamic runtime, using extra DLL, thus changes will be reflected when building sandbox

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mkpch.h"
    pchsource "src/mkpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/stb_image/**.h",
        "vendor/stb_image/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING"
    }
    
    includedirs
    {
        "vendor/spdlog/include",
        "src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}"
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
        links "glfw3"

    filter "configurations:Dist"  
        defines "MK_DIST"
        runtime "Release"
        optimize "On"
        links "glfw3"
