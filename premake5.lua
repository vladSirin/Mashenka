include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Mashenka"
    architecture "x64"
    startproject "Tamara"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile" -- enable multi-processor compilation --
    
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory) --
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Mashenka/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Mashenka/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Mashenka/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Mashenka/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Mashenka/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Mashenka/vendor/entt/include"

-- Include the premake config for GLFW from the submodule, which contains how to build it --
-- Group Dependencies
group "Dependencies"
    include "vendor/premake"
    include "Mashenka/vendor/GLFW"
    include "Mashenka/vendor/Glad"
    include "Mashenka/vendor/imgui"

group ""
    include "Mashenka"
    include "Sandbox"
    include "Tamara"
