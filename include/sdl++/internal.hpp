#pragma once

#include <memory>

namespace sdl::internal {

template <class T, class Ret, Ret(*deleter)(T*)>
class Holder {
public:
    Holder() = default;

    explicit Holder(T* ptr)
    {
        _ptr.reset(ptr);
    }

    explicit operator bool() const
    {
        return _ptr;
    }

    operator T*()
    {
        return _ptr.get();
    }

    operator const T*() const
    {
        return _ptr.get();
    }

private:
    std::unique_ptr<T, Ret(*)(T*)> _ptr{nullptr, deleter};
};

} // namespace sdl::internal