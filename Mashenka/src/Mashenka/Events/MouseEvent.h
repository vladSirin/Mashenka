#pragma once
#include "Event.h"

namespace Mashenka
{
    class MASHENKA_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y)
        {
        }

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent" << m_MouseX << "," << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

        // This is a bitwise operation to create a new category by combining both of them
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_MouseX, m_MouseY;
    };

    class MASHENKA_API MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset)
        {
        }

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float m_XOffset, m_YOffset;
    };

    // This is the base class for Mouse Button Events
    // We will need button pressed and release events inheriting from it
    class MASHENKA_API MouseButtonEvent : public Event
    {
    public:
        inline int GetMouseButton() const {return m_Button;}

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
        
    protected:
        MouseButtonEvent(int button)
            : m_Button(button) {}
        int m_Button;
    };

    class MASHENKA_API MouseButtonPressedEvent: public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };
}
