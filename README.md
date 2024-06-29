# Mashenka
Mashenka Engine by Sirin

![Mashenka](/Resources/Branding/MashenkaLogo.png?raw=true "Mashenka")
Mashenka is a game engine based on Hazel Engine by TheCherno. It is a work in progress and is not ready for use.

## Getting Started
Visual Studio 2022 or Rider is recommended, Mashenka is officially untested on other development environments whilst we focus on a Windows build.

You can clone the repository to a local destination using git:

`git clone --recursive https://github.com/vladSirin/Mashenka`

Make sure that you do a `--recursive` clone to fetch all submodules!

## Setting up the solution:
- Download the latest premake pre-built version from https://premake.github.io/download or build it yourself.
- Copy the premake files under `{localRepoName}\vendor\bin\premake`, create the folder if it does not exist.
- The folder should not be included into the version control, if it is not the case, try add the path to the `.gitignore` file.
- Go to the Mashenka directory and run `scripts\Win-GenProjects.bat` to generate the Visual Studio solution.
- Open the solution in Visual Studio or any other IDE you are using and build the `Mashenka` target.
- Set the `Sandbox` project as the startup project and run.
- Make sure to set the working directory to `Mashenka\Sandbox\assets` in the project settings.
- If you get an error about missing DLLs, make sure you have the Visual C++ Runtime installed.
- If having LINKER errors, make sure you setup the multi-threaded debugger (no DLL) runtime library for GLFW and all other projects (included dependencies, Glad/GLFW/ImGui etc.)

## The Plan
This is a demo engine for me to mainly learning Game Engine Architecture and C++.
Plan to finish basic 2D game functionality by the end of 2023 and create first demo game.

### Main features to come:
- Demo Game Project
- Fast 2D rendering (UI, particles, sprites, etc.)
- High-fidelity Physically-Based 3D rendering (this will be expanded later, 2D to come first)
- Support for Mac, Linux, Android and iOS
    - Native rendering API support (DirectX, Vulkan, Metal)
- Fully featured viewer and editor applications
- Fully scripted interaction and behavior
- Integrated 3rd party 2D and 3D physics engine
- Procedural terrain and world generation
- Artificial Intelligence
- Audio system
