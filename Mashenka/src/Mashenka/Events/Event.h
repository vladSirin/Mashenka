#pragma once

#include "Mashenka/Core/Base.h"

namespace Mashenka
{
    // Events in Mashenka are currently blocking, meaning when an event occurs it
    // Immediately gets dispatched and must be dealt with right then and there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage

    // Enumeration that lists all the possible types of events that can occur in the game engine
    enum class EventType
    {
        None = 0,
        WindowsClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // Enumeration that lists all the categories of events for the game engine
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4),
    };

    // define common functions in event classes, they help to reduce code redundancy
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
                               virtual EventType GetEventType() const override {return GetStaticType();}\
                               virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

    // base event class for all events in the engine
    class Event
    {
        // The friend class can access the private and protected data from this
        // friend class EventDispatcher;
    public:
        /* Proper resource management: In a polymorphic base class, if you delete an object through a base class ptr
         * the base class must have a virtual destructor to ensure that the derived class's destructor is called. This
         * ensures that any resources allocated by the derived class are properly released
         * Avoiding Undefined Behavior: Without a virtual destructor in a base class, deleting an object of deprived
         * class through a base class pointer results in undefined behavior, leading to potential resource leaks or crashes
         * Compiler Optimization: using "= default;" allows the compiler to optimize the destructor. It can also make
         * the code more readable and concise, signaling that the default behavior is intended.
         */
        virtual ~Event() = default;

        bool Handled = false;

        // virtual functions that must be overriden by derived classes
        // Those functions are macro-ed so that it can easily created
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        // Check if an event belongs to a certain category
        inline bool IsInCategory(EventCategory category)
        {
            // This is a bitwise operation to check if the two categories match by comparing
            // every bit number for the two int
            return GetCategoryFlags() & category;
        }

    protected:
        // indicate that if the event has been handled or not
        // bool m_Handled = false;
    };

    // This class is to dispatch events
    class EventDispatcher
    {
        // This is the constructor
    public:
        EventDispatcher(Event& event)
        // Initialize the member variable 'm_Event'
            : m_Event(event)
        {
        }

        // This is the template function that will be called by the application
        // EventType is the type of the event, Func is the function that will be called
        // Two types are used here, one is the type of the event, the other is the type of the function
        template <typename EventType, typename Func>

        // Dispatch the event, Func&& means that the function is a rvalue reference, which means that the function will be called directly
        // Using rvalue reference is to avoid copying the function, which is expensive
        bool Dispatch(Func&& func)
        {
            // Check if the event type matches the type of the event
            if (m_Event.GetEventType() == EventType::GetStaticType())
            {
                // If so, call the function
                // static_cast is used to convert the type of the event, which is Event, to the type of the event, which is EventType
                m_Event.Handled = func(static_cast<EventType&>(m_Event)); // Call the function
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
