#pragma once

namespace Mashenka
{
    // Create enum class for key codes of all keys, using uint16_t as the underlying type
    // The key codes are based on the GLFW key codes, which are based on the USB HID key codes
    // from 32 to 348
    enum class KeyCode : uint16_t
    {
        // The unknown key
        Unknown = 0,

        // Printable keys
        Space = 32,
        Apostrophe = 39, /* ' */
        Comma = 44, /* , */
        Minus = 45, /* - */
        Period = 46, /* . */
        Slash = 47, /* / */
        Num0 = 48,
        Num1 = 49,
        Num2 = 50,
        Num3 = 51,
        Num4 = 52,
        Num5 = 53,
        Num6 = 54,
        Num7 = 55,
        Num8 = 56,
        Num9 = 57,
        Semicolon = 59, /* ; */
        Equal = 61, /* = */
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91, /* [ */
        Backslash = 92, /* \ */
        RightBracket = 93, /* ] */
        GraveAccent = 96, /* ` */
        World1 = 161, /* non-US #1 */
        World2 = 162, /* non-US #2 */

        // Function keys
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        Keypad0 = 320,
        Keypad1 = 321,
        Keypad2 = 322,
        Keypad3 = 323,
        Keypad4 = 324,
        Keypad5 = 325,
        Keypad6 = 326,
        Keypad7 = 327,
        Keypad8 = 328,
        Keypad9 = 329,
        KeypadDecimal = 330,
        KeypadDivide = 331,
        KeypadMultiply = 332,
        KeypadSubtract = 333,
        KeypadAdd = 334,
        KeypadEnter = 335,
        KeypadEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };
    

    // Create alias for key codes
    typedef KeyCode Key;

    // Overload the << operator to print the key code
    // usage: std::cout << KeyCode::Space will print 32
    inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
    {
        os << static_cast<int32_t>(keyCode);
        return os;
    }

    // define marcos for the enums of all key codes
    // from space to menu
    #define MK_KEY_SPACE           ::Mashenka::KeyCode::Space
    #define MK_KEY_APOSTROPHE      ::Mashenka::KeyCode::Apostrophe    /* ' */
    #define MK_KEY_COMMA           ::Mashenka::KeyCode::Comma         /* , */
    #define MK_KEY_MINUS           ::Mashenka::KeyCode::Minus         /* - */
    #define MK_KEY_PERIOD          ::Mashenka::KeyCode::Period        /* . */
    #define MK_KEY_SLASH           ::Mashenka::KeyCode::Slash         /* / */
    #define MK_KEY_0               ::Mashenka::KeyCode::Num0
    #define MK_KEY_1               ::Mashenka::KeyCode::Num1
    #define MK_KEY_2               ::Mashenka::KeyCode::Num2
    #define MK_KEY_3               ::Mashenka::KeyCode::Num3
    #define MK_KEY_4               ::Mashenka::KeyCode::Num4
    #define MK_KEY_5               ::Mashenka::KeyCode::Num5
    #define MK_KEY_6               ::Mashenka::KeyCode::Num6
    #define MK_KEY_7               ::Mashenka::KeyCode::Num7
    #define MK_KEY_8               ::Mashenka::KeyCode::Num8
    #define MK_KEY_9               ::Mashenka::KeyCode::Num9
    #define MK_KEY_SEMICOLON       ::Mashenka::KeyCode::Semicolon     /* ; */
    #define MK_KEY_EQUAL           ::Mashenka::KeyCode::Equal         /* = */
    #define MK_KEY_A               ::Mashenka::KeyCode::A
    #define MK_KEY_B               ::Mashenka::KeyCode::B
    #define MK_KEY_C               ::Mashenka::KeyCode::C
    #define MK_KEY_D               ::Mashenka::KeyCode::D
    #define MK_KEY_E               ::Mashenka::KeyCode::E
    #define MK_KEY_F               ::Mashenka::KeyCode::F
    #define MK_KEY_G               ::Mashenka::KeyCode::G
    #define MK_KEY_H               ::Mashenka::KeyCode::H
    #define MK_KEY_I               ::Mashenka::KeyCode::I
    #define MK_KEY_J               ::Mashenka::KeyCode::J
    #define MK_KEY_K               ::Mashenka::KeyCode::K
    #define MK_KEY_L               ::Mashenka::KeyCode::L
    #define MK_KEY_M               ::Mashenka::KeyCode::M
    #define MK_KEY_N               ::Mashenka::KeyCode::N
    #define MK_KEY_O               ::Mashenka::KeyCode::O
    #define MK_KEY_P               ::Mashenka::KeyCode::P
    #define MK_KEY_Q               ::Mashenka::KeyCode::Q
    #define MK_KEY_R               ::Mashenka::KeyCode::R
    #define MK_KEY_S               ::Mashenka::KeyCode::S
    #define MK_KEY_T               ::Mashenka::KeyCode::T
    #define MK_KEY_U               ::Mashenka::KeyCode::U
    #define MK_KEY_V               ::Mashenka::KeyCode::V
    #define MK_KEY_W               ::Mashenka::KeyCode::W
    #define MK_KEY_X               ::Mashenka::KeyCode::X
    #define MK_KEY_Y               ::Mashenka::KeyCode::Y
    #define MK_KEY_Z               ::Mashenka::KeyCode::Z
    #define MK_KEY_LEFT_BRACKET    ::Mashenka::KeyCode::LeftBracket   /* [ */
    #define MK_KEY_BACKSLASH       ::Mashenka::KeyCode::Backslash     /* \ */
    #define MK_KEY_RIGHT_BRACKET   ::Mashenka::KeyCode::RightBracket  /* ] */
    #define MK_KEY_GRAVE_ACCENT    ::Mashenka::KeyCode::GraveAccent   /* ` */
    #define MK_KEY_WORLD_1         ::Mashenka::KeyCode::World1        /* non-US #1 */
    #define MK_KEY_WORLD_2         ::Mashenka::KeyCode::World2        /* non-US #2 */
    #define MK_KEY_ESCAPE          ::Mashenka::KeyCode::Escape
    #define MK_KEY_ENTER           ::Mashenka::KeyCode::Enter
    #define MK_KEY_TAB             ::Mashenka::KeyCode::Tab
    #define MK_KEY_BACKSPACE       ::Mashenka::KeyCode::Backspace
    #define MK_KEY_INSERT          ::Mashenka::KeyCode::Insert
    #define MK_KEY_DELETE          ::Mashenka::KeyCode::Delete
    #define MK_KEY_RIGHT           ::Mashenka::KeyCode::Right
    #define MK_KEY_LEFT            ::Mashenka::KeyCode::Left
    #define MK_KEY_DOWN            ::Mashenka::KeyCode::Down
    #define MK_KEY_UP              ::Mashenka::KeyCode::Up
    #define MK_KEY_PAGE_UP         ::Mashenka::KeyCode::PageUp
    #define MK_KEY_PAGE_DOWN       ::Mashenka::KeyCode::PageDown
    #define MK_KEY_HOME            ::Mashenka::KeyCode::Home
    #define MK_KEY_END             ::Mashenka::KeyCode::End
    #define MK_KEY_CAPS_LOCK       ::Mashenka::KeyCode::CapsLock
    #define MK_KEY_SCROLL_LOCK     ::Mashenka::KeyCode::ScrollLock
    #define MK_KEY_NUM_LOCK        ::Mashenka::KeyCode::NumLock
    #define MK_KEY_PRINT_SCREEN    ::Mashenka::KeyCode::PrintScreen
    #define MK_KEY_PAUSE           ::Mashenka::KeyCode::Pause
    #define MK_KEY_F1              ::Mashenka::KeyCode::F1
    #define MK_KEY_F2              ::Mashenka::KeyCode::F2
    #define MK_KEY_F3              ::Mashenka::KeyCode::F3
    #define MK_KEY_F4              ::Mashenka::KeyCode::F4
    #define MK_KEY_F5              ::Mashenka::KeyCode::F5
    #define MK_KEY_F6              ::Mashenka::KeyCode::F6
    #define MK_KEY_F7              ::Mashenka::KeyCode::F7
    #define MK_KEY_F8              ::Mashenka::KeyCode::F8
    #define MK_KEY_F9              ::Mashenka::KeyCode::F9
    #define MK_KEY_F10             ::Mashenka::KeyCode::F10
    #define MK_KEY_F11             ::Mashenka::KeyCode::F11
    #define MK_KEY_F12             ::Mashenka::KeyCode::F12
    #define MK_KEY_F13             ::Mashenka::KeyCode::F13
    #define MK_KEY_F14             ::Mashenka::KeyCode::F14
    #define MK_KEY_F15             ::Mashenka::KeyCode::F15
    #define MK_KEY_F16             ::Mashenka::KeyCode::F16
    #define MK_KEY_F17             ::Mashenka::KeyCode::F17
    #define MK_KEY_F18             ::Mashenka::KeyCode::F18
    #define MK_KEY_F19             ::Mashenka::KeyCode::F19
    #define MK_KEY_F20             ::Mashenka::KeyCode::F20
    #define MK_KEY_F21             ::Mashenka::KeyCode::F21
    #define MK_KEY_F22             ::Mashenka::KeyCode::F22
    #define MK_KEY_F23             ::Mashenka::KeyCode::F23
    #define MK_KEY_F24             ::Mashenka::KeyCode::F24
    #define MK_KEY_F25             ::Mashenka::KeyCode::F25
    #define MK_KEY_KEYPAD_0        ::Mashenka::KeyCode::Keypad0
    #define MK_KEY_KEYPAD_1        ::Mashenka::KeyCode::Keypad1
    #define MK_KEY_KEYPAD_2        ::Mashenka::KeyCode::Keypad2
    #define MK_KEY_KEYPAD_3        ::Mashenka::KeyCode::Keypad3
    #define MK_KEY_KEYPAD_4        ::Mashenka::KeyCode::Keypad4
    #define MK_KEY_KEYPAD_5        ::Mashenka::KeyCode::Keypad5
    #define MK_KEY_KEYPAD_6        ::Mashenka::KeyCode::Keypad6
    #define MK_KEY_KEYPAD_7        ::Mashenka::KeyCode::Keypad7
    #define MK_KEY_KEYPAD_8        ::Mashenka::KeyCode::Keypad8
    #define MK_KEY_KEYPAD_9        ::Mashenka::KeyCode::Keypad9
    #define MK_KEY_KEYPAD_DECIMAL  ::Mashenka::KeyCode::KeypadDecimal
    #define MK_KEY_KEYPAD_DIVIDE   ::Mashenka::KeyCode::KeypadDivide
    #define MK_KEY_KEYPAD_MULTIPLY ::Mashenka::KeyCode::KeypadMultiply
    #define MK_KEY_KEYPAD_SUBTRACT ::Mashenka::KeyCode::KeypadSubtract
    #define MK_KEY_KEYPAD_ADD      ::Mashenka::KeyCode::KeypadAdd
    #define MK_KEY_KEYPAD_ENTER    ::Mashenka::KeyCode::KeypadEnter
    #define MK_KEY_KEYPAD_EQUAL    ::Mashenka::KeyCode::KeypadEqual
    #define MK_KEY_LEFT_SHIFT      ::Mashenka::KeyCode::LeftShift
    #define MK_KEY_LEFT_CONTROL    ::Mashenka::KeyCode::LeftControl
    #define MK_KEY_LEFT_ALT        ::Mashenka::KeyCode::LeftAlt
    #define MK_KEY_LEFT_SUPER      ::Mashenka::KeyCode::LeftSuper
    #define MK_KEY_RIGHT_SHIFT     ::Mashenka::KeyCode::RightShift
    #define MK_KEY_RIGHT_CONTROL   ::Mashenka::KeyCode::RightControl
    #define MK_KEY_RIGHT_ALT       ::Mashenka::KeyCode::RightAlt
    #define MK_KEY_RIGHT_SUPER     ::Mashenka::KeyCode::RightSuper
    #define MK_KEY_MENU            ::Mashenka::KeyCode::Menu
    #define MK_KEY_LAST            ::Mashenka::KeyCode::Menu
   
}
