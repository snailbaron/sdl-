#pragma once

#include <sdl++/internal.hpp>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <string>

namespace sdl {

class Init {
public:
    Init(uint32_t flags);
    ~Init();

    Init(const Init&) = delete;
    Init& operator=(const Init&) = delete;

    Init(Init&&) noexcept = delete;
    Init& operator=(Init&&) noexcept = delete;

private:
    uint32_t _flags = 0;
};

class IOStream : public internal::Holder<SDL_IOStream, bool, SDL_CloseIO> {
    using Holder::Holder;

    explicit IOStream(std::span<const std::byte> mem);
    explicit IOStream(std::span<std::byte> mem);

    void flush();
    size_t size();
};

class Surface : public internal::Holder<SDL_Surface, void, SDL_DestroySurface> {
    using Holder::Holder;
};

class Texture : public internal::Holder<SDL_Texture, void, SDL_DestroyTexture> {
    using Holder::Holder;
};

class Renderer : public internal::Holder<SDL_Renderer, void, SDL_DestroyRenderer> {
public:
    using Holder::Holder;

    void present();

    Texture loadTexture(const std::filesystem::path& file);
    Texture loadTexture(std::span<const std::byte> mem);
};

class Window : public internal::Holder<SDL_Window, void, SDL_DestroyWindow> {
public:
    Window(const std::string& title, int w, int h, uint32_t flags);

    Renderer createRenderer(const std::string& name);
};

} // namespace sdl

namespace img {

class Init {
    explicit Init(IMG_InitFlags flags);
    ~Init();

    Init(const Init&) = delete;
    Init& operator=(const Init&) = delete;

    Init(Init&&) noexcept = delete;
    Init& operator=(Init&&) noexcept = delete;
};

sdl::Surface load(const std::filesystem::path& file);

} // namespace img