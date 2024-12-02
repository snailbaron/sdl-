#include <sdl++/error.hpp>

#include <format>
#include <stacktrace>

namespace sdl {

Error::Error(std::source_location sl)
    : Error("", sl)
{ }

Error::Error(std::string_view message, std::source_location sl)
    : _message(std::format(
        "{}:{}:{} ({}): {}\n{}",
        sl.file_name(),
        sl.line(),
        sl.column(),
        sl.function_name(),
        message,
        std::stacktrace::current()))
{ }

const char* Error::what() const noexcept
{
    return _message.c_str();
}

} // namespace sdl