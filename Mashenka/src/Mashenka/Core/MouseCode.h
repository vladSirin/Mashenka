#pragma once

namespace Mashenka
{

    // Create alias for mouse button codes
    enum class MouseCode : uint16_t
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,
        ButtonLast = Button7,
        ButtonLeft = Button0,
        ButtonRight = Button1,
        ButtonMiddle = Button2
    };

    //typedef MouseCode Mouse;
    typedef MouseCode Mouse;

    // Overload the << operator to print the mouse code
    // usage: std::cout << MouseCode::Button0 will print 0
    inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
    {
        os << static_cast<int32_t>(mouseCode);
        return os;
    }

    // define marcos for the enums of all mouse code buttons
    // from mouse button 0 to mouse button 7 and functional buttons
    #define MK_MOUSE_BUTTON_0      ::Mashenka::MouseCode::Button0
    #define MK_MOUSE_BUTTON_1      ::Mashenka::MouseCode::Button1
    #define MK_MOUSE_BUTTON_2      ::Mashenka::MouseCode::Button2
    #define MK_MOUSE_BUTTON_3      ::Mashenka::MouseCode::Button3
    #define MK_MOUSE_BUTTON_4      ::Mashenka::MouseCode::Button4
    #define MK_MOUSE_BUTTON_5      ::Mashenka::MouseCode::Button5
    #define MK_MOUSE_BUTTON_6      ::Mashenka::MouseCode::Button6
    #define MK_MOUSE_BUTTON_7      ::Mashenka::MouseCode::Button7
    #define MK_MOUSE_BUTTON_LAST   ::Mashenka::MouseCode::ButtonLast
    #define MK_MOUSE_BUTTON_LEFT   ::Mashenka::MouseCode::ButtonLeft
    #define MK_MOUSE_BUTTON_RIGHT  ::Mashenka::MouseCode::ButtonRight
    #define MK_MOUSE_BUTTON_MIDDLE ::Mashenka::MouseCode::ButtonMiddle

    
}
