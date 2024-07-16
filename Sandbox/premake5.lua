project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"
    cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    defines
    {
        "_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING",
        "_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
    }
    
    includedirs
    {
        "%{wks.location}/Mashenka/vendor/spdlog/include",
        "%{wks.location}/Mashenka/src",
        "%{IncludeDir.glm}",
        "%{wks.location}/Mashenka/vendor",
        "%{IncludeDir.entt}"
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
