#pragma once
#include "Mashenka/Core/Base.h"
#include "Mashenka/Core/KeyCodes.h"
#include "Mashenka/Core/MouseCode.h"

// This is the input System that will be called by the application loop
// Using a Singleton Interface Pattern
namespace Mashenka
{
    class Input
    {
    protected:
        // This is the constructor of the Input class
        Input() = default;

    public:
        // This is the declaration of the singleton instance of the Input class
        Input(const Input&) = delete; // forbid copy constructor
        Input& operator=(const Input&) = delete; // forbid copy assignment operator

        inline static bool IsKeyPressed(Key keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        inline static bool IsMouseButtonPressed(Mouse button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

        inline static void Poll() { s_Instance->PollImpl(); }

        static Scope<Input> Create();

        /* Proper resource management: In a polymorphic base class, if you delete an object through a base class ptr
         * the base class must have a virtual destructor to ensure that the derived class's destructor is called. This
         * ensures that any resources allocated by the derived class are properly released
         * Avoiding Undefined Behavior: Without a virtual destructor in a base class, deleting an object of deprived
         * class through a base class pointer results in undefined behavior, leading to potential resource leaks or crashes
         * Compiler Optimization: using "= default;" allows the compiler to optimize the destructor. It can also make
         * the code more readable and concise, signaling that the default behavior is intended.
         */
        virtual ~Input() = default;

    protected:
        // All impl functions are declared =0 which you need to provide implementation in derived class
        virtual void PollImpl() = 0;
        virtual bool IsKeyPressedImpl(Key keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(Mouse button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        // This is a singleton declare, need to be initialized in cpp
        static Scope<Input> s_Instance;
    };
}
