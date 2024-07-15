#pragma once

#include "Mashenka/Events/Event.h"
#include "Mashenka/Core/KeyCodes.h"

// For key events, there should be key pressed and releases as major public classes
// As the normal logic for applications are based on those too
// Repeat, normally should be a function that is delayed upon the key pressed event.
// And based on how long u pressed it to count the num of repeats that should happen
namespace Mashenka
{
    class KeyEvent: public Event
    {
    public:
        inline KeyCode GetKeyCode() const {return  m_KeyCode;}

        EVENT_CLASS_CATEGORY(EventCategoryApplication | EventCategoryInput)

    protected:
        KeyEvent(const KeyCode keycode)
            : m_KeyCode(keycode){}

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent: public KeyEvent
    {
    public:
        KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
            :KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        uint16_t GetRepeatCount() const {return m_RepeatCount;}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent" << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        uint16_t m_RepeatCount;
    };

    class KeyReleasedEvent: public KeyEvent
    {
    public:
        KeyReleasedEvent(const KeyCode keycode)
            :KeyEvent(keycode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)

    };

    //DONE: Implement a key typed function
    class KeyTypedEvent: public KeyEvent
    {
    public:
        KeyTypedEvent(const KeyCode keycode)
            :KeyEvent(keycode){}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
