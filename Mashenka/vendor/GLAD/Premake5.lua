project "Glad"
kind "StaticLib"
language "C"
staticruntime "off"
warnings "off"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

files
{
    "include/Glad/glad.h",
    "include/KHR/khrplatform.h",
    "src/glad.c"
}

includedirs
{
    "include"
}

filter {"system:windows", "configurations:Release"}
    buildoptions "/MT"

filter "system:linux"
pic "On"
systemversion "latest"

filter "system:macosx"
pic "On"

filter "system:windows"
systemversion "latest"
staticruntime "On"

filter "configurations:Debug"
runtime "Debug"
symbols "on"

filter "configurations:Release"
runtime "Release"
optimize "speed"

filter "configurations:Dist"
runtime "Release"
optimize "speed"
symbols "off"