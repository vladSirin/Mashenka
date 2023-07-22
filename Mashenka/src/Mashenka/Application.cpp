#include "Application.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Log.h"

namespace Mashenka
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    /*
     * 4. **Running the Application**:
     * The application is run by calling the `run` function on the application instance.
     * This initiates a while loop that keeps the application running.
     */
    void Application::Run()
    {
        {
            WindowResizeEvent e(1280, 270);
            if(e.IsInCategory(EventCategoryApplication))
            {
                MK_TRACE(e);
            }

            if(e.IsInCategory(EventCategoryInput))
            {
                MK_TRACE(e);
            }
        }
        while (true);
    }
}
