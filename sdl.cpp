#include <sdl.hpp>

#include <sdl++/error.hpp>

#include <mutex>

namespace sdl {

namespace {

size_t initCount = 0;
std::mutex initCountMutex;

void check(bool success)
{
    if (!success) {
        throw Error{SDL_GetError()};
    }
}

template <class T>
T* check(T* ptr)
{
    if (!ptr) {
        throw Error{SDL_GetError()};
    }
    return ptr;
}

} // namespace

Init::Init(uint32_t flags)
    : _flags(flags)
{
    auto lock = std::lock_guard{initCountMutex};
    check(SDL_Init(_flags));
    initCount++;
}

Init::~Init()
{
    auto lock = std::lock_guard{initCountMutex};
    SDL_QuitSubSystem(_flags);
    initCount--;
    if (initCount == 0) {
        SDL_Quit();
    }
}

IOStream::IOStream(std::span<const std::byte> mem)
    : Holder(check(SDL_IOFromConstMem(mem.data(), mem.size())))
{ }

IOStream::IOStream(std::span<std::byte> mem)
    : Holder(check(SDL_IOFromMem(mem.data(), mem.size())))
{ }

void IOStream::flush()
{
    check(SDL_FlushIO(*this));
}

size_t IOStream::size()
{
    auto size = SDL_GetIOSize(*this);
    if (size < 0) {
        throw Error{SDL_GetError()};
    }
    return size;
}

void Renderer::present()
{
    check(SDL_RenderPresent(*this));
}

Texture Renderer::loadTexture(const std::filesystem::path& file)
{
    return Texture{check(IMG_LoadTexture(*this, file.string().c_str()))};
}

Texture Renderer::loadTexture(std::span<const std::byte> mem)
{
    SDL_IOStream* stream = check(SDL_IOFromConstMem(mem.data(), mem.size()));
    return Texture{check(IMG_LoadTexture_IO(*this, stream, true))};
}

Window::Window(const std::string& title, int w, int h, uint32_t flags)
    : Holder(check(SDL_CreateWindow(title.c_str(), w, h, flags)))
{ }

Renderer Window::createRenderer(const std::string& name)
{
    return Renderer{check(SDL_CreateRenderer(*this, name.c_str()))};
}

} // namespace sdl

namespace img {

namespace {

size_t initCount = 0;
std::mutex initCountMutex;

} // namespace

Init::Init(IMG_InitFlags flags)
{
    auto lock = std::lock_guard{initCountMutex};
    if (IMG_Init(flags) != flags) {
        throw sdl::Error{SDL_GetError()};
    }
    initCount++;
}

Init::~Init()
{
    auto lock = std::lock_guard{initCountMutex};
    initCount--;
    if (initCount == 0) {
        IMG_Quit();
    }
}

sdl::Surface load(const std::filesystem::path& file)
{
    return sdl::Surface{sdl::check(IMG_Load(file.string().c_str()))};
}

} // namespace img