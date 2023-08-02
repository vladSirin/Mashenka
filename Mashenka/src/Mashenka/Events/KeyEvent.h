#pragma once

#include "Event.h"

// For key events, there should be key pressed and releases as major public classes
// As the normal logic for applications are based on those too
// Repeat, normally should be a function that is delayed upon the key pressed event.
// And based on how long u pressed it to count the num of repeats that should happen
namespace Mashenka
{
    class MASHENKA_API KeyEvent: public Event
    {
    public:
        inline int GetKeyCode() const {return  m_KeyCode;}

        EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryInput)

    protected:
        KeyEvent(int Keycode)
            : m_KeyCode(Keycode){}

        int m_KeyCode;
    };

    class MASHENKA_API KeyPressedEvent: public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount)
            :KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const {return m_RepeatCount;}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent" << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_RepeatCount;
    };

    class MASHENKA_API KeyReleasedEvent: public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode)
            :KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)

    };
}
