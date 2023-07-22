#pragma once

#include "Mashenka/Core.h"
#include <string>
#include <functional>

#include "Mashenka/Log.h"

namespace Mashenka
{
    // Events in Mashenka are currently blocking, meaning when an event occurs it
    // Immediately gets dispatched and must be dealt with right then and there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage

    // Enumeration that lists all the possible types of events that can occur in the game engine
    enum class EventType
    {
        None=0,
        WindowsClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // Enumeration that lists all the categories of events for the game engine
    enum EventCategory
    {
        None=0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4),
    };

    // define common functions in event classes, they help to reduce code redundancy
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
                               virtual EventType GetEventType() const override {return GetStaticType();}\
                               virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

    // base event class for all events in the engine
    class MASHENKA_API Event
    {
        friend class EventDispatcher;
    public:

        // virtual functions that must be overriden by derived classes
        // Those functions are macro-ed so that it can easily created
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const {return GetName();}

        // Check if an event belongs to a certain category
        inline bool IsInCategory(EventCategory category)
        {
            // This is a bitwise operation to check if the two categories match by comparing
            // every bit number for the two int
            return GetCategoryFlags() & category;
        }
    protected:

        // indicate that if the event has been handled or not
        bool m_Handled = false;
    };

    // This class is to dispatch events
    class EventDispatcher
    {
        // In this case, template<typename T> declares a template that takes a single type parameter T.
        // You can think of T as a placeholder for a type that will be provided later.
        // When you use this template to define a function or a class,
        // you can use T as if it were a regular type.
        template<typename T>

        //naming alias for a std::function that takes a ref to an object of type 'T' and returns a 'bool'
        // The power of std::function comes from its ability to wrap any kind of callable entity,
        // not just simple functions. 
        using EventFn = std::function<bool(T&)>;
        
    // This is the constructor
    public:
        EventDispatcher(Event& event)
            // Initialize the member variable 'm_Event'
            : m_Event(event)
        {
            
        }

        template<typename T>

        // Takes an function as input
        bool Dispatch(EventFn<T> func)
        {
            // if the event type matches the "func" type supposed to be handled
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                // calls 'func' with the event and sets m_handled to the result
                m_Event.m_Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };

    // '<<' operator is overloaded for 'std::ostream'
    // events to be directly output to streams like 'std::cout', so that:
    /* The following the code:
    Event e;
    std::cout << e.ToString();

    Can we simplified to below:
    Event e;
    std::cout << e;
    */
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}
