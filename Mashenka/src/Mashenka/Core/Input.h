#pragma once
#include "Mashenka/Core/Core.h"

// This is the input System that will be called by the application loop
// Using a Singleton Interface Pattern
namespace Mashenka
{
    class MASHENKA_API Input
    {
    protected:
        // This is the constructor of the Input class
        Input() = default;
    public:
        // This is the declaration of the singleton instance of the Input class
        Input(const Input&) = delete; // forbid copy constructor
        Input& operator=(const Input&) = delete; // forbid copy assignment operator
        
        inline static bool IsKeyPressed(int keycode) {return s_Instance->IsKeyPressedImpl(keycode);}
        inline static bool IsMouseButtonPressed(int button) {return s_Instance->IsMouseButtonPressedImpl(button);}
        inline static std::pair<float, float> GetMousePosition() {return s_Instance->GetMousePositionImpl();}
        inline static float GetMouseX() {return s_Instance->GetMouseXImpl();}
        inline static float GetMouseY() {return s_Instance->GetMouseYImpl();}

        inline static void Poll() {s_Instance->PollImpl();}
        
        // A inline destructor is good practice
        virtual ~Input() {}

    protected:
        // All impl functions are declared =0 which you need to provide implementation in derived class
        virtual void PollImpl() = 0;
        virtual bool IsKeyPressedImpl(int keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float,float> GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
        
    private:
        // This is a singleton declare, need to be initialized in cpp
        static std::unique_ptr<Input> s_Instance;
    };
    
}

