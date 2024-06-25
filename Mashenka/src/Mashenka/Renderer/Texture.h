#pragma once
#include <string>
#include "Mashenka/Core/Base.h"

namespace Mashenka
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        // Bind the texture to a slot, default is 0, which is the first slot
        // This is used when we have multiple textures, slots are used to distinguish them
        // For example, we have a texture for the color of the object, and a texture for the normal of the object
        // We can bind the color texture to slot 0, and the normal texture to slot 1
        // Then we can use the color texture in the shader by using the sampler2D with slot 0
        // And we can use the normal texture in the shader by using the sampler2D with slot 1
        virtual void Bind(uint32_t slot = 0) const = 0;

        virtual bool operator==(const Texture& other) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        // Create
        static Ref<Texture2D> Create(const std::string& path);
        static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        // create a texture with the given width and height
    };
}
