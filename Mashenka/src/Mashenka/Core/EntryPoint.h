#pragma once
#include "Mashenka/Core/Core.h"

/*
 * 2. **Creating an Entry Point**: An entry point is created in the Hazel engine.
 * This is where the application starts.
 * The entry point is defined in a header file called "entry point".
 * This file contains a function that returns a Hazel application pointer.
 * This function is defined elsewhere and is used to create an application.
 */

// This is a common design pattern called, Platform layer or Application Framework
#ifdef MK_PLATFORM_WINDOWS

extern Mashenka::Application* Mashenka::CreateApplication();

int main(int argc, char** argv) //main cannot be inline
{
    Mashenka::Log::Init();
    MK_CORE_WARN("Initialized Log!");
    auto a = "User";
    MK_CORE_INFO("Hello {0}, Welcome to Mashenka!", a);

    MK_PROFILE_BEGIN_SESSION("Startup", "profile-data/MashenkaProfile-Startup.json");
    auto app = Mashenka::CreateApplication();
    MK_PROFILE_END_SESSION();

    // Profile
    MK_PROFILE_BEGIN_SESSION("Runtime", "profile-data/MashenkaProfile-Runtime.json");
    app->Run();
    MK_PROFILE_END_SESSION();

    // Profile
    MK_PROFILE_BEGIN_SESSION("Shutdown", "profile-data/MashenkaProfile-Shutdown.json");
    delete app;
    MK_PROFILE_END_SESSION();
}

#endif