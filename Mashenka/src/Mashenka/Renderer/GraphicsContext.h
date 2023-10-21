#pragma once


namespace Mashenka
{
    // Building a high level Graphic context interface so the platform specific APIs can be called in a sub-class like OpenGLContext.h
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0 ;
    };
}
