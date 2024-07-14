#pragma once
#include "Mashenka/Core/Base.h"

namespace Mashenka
{
    // Specs for the framebuffer, later used in creating the frame buffer
    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0; //initialize variables
        // FramebufferFormat Format =
        uint32_t Samples = 1; // MSAA sample

        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        // bind a framebuffer in opengl is telling opengl to direct all subsequent rendering operations to that framebuffer
        // normally those operations will be direct to the default framebuffer which is the one displayed right now
        virtual ~Framebuffer() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        // resize the framebuffer
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        /* @brief: In OpenGL, framebuffers can have multiple types of attachments,
         * each serving different purposes for rendering. The most common attachments are color attachments,
         * depth attachments, and stencil attachments. Here’s a detailed explanation of each:
        * Color Attachment: Stores the color data of the rendered scene. It contains the final color output for each pixel.
        * Depth Attachment: Stores depth information for each pixel, used for depth testing to determine visibility.
        * Stencil Attachment: Stores stencil information for stencil testing, used for advanced rendering techniques.
        * Depth-Stencil Attachment: Combines depth and stencil information into a single texture or renderbuffer.
         */
        virtual uint32_t GetColorAttachmentRendererID() const = 0; // Getting the final color output for each pixel through color attachemtn
        virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}
