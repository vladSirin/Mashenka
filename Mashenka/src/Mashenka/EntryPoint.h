#pragma once

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

inline int main(int argc, char** argv)
{
    printf("Welcome to mashenka engine!\n");
    auto app = Mashenka::CreateApplication();
    app->Run();
    delete app;
}

#endif