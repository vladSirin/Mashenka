#pragma once
#include "Mashenka/Core/KeyCodes.h"
#include "Mashenka/Core/MouseCode.h"
#include <glm/glm.hpp>

/* All functions are static and public, thus we do not need an instance to call those functions
 * Apparently input platform will not be changed on runtime or from time to time, this means it can be compiled based
 * on the platform it will be shipped, thus this approach
 */
// This is the input System that will be called by the application loop
// Using a Static Class pattern.
namespace Mashenka
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static glm::vec2 GetMousePosition(); // using glm::vec2 for easier usage and consistency
        static float GetMouseX();
        static float GetMouseY();
    };
}
