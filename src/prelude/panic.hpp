#ifndef PANIC_HPP
#define PANIC_HPP

#include <cstdlib>
#include <format>
#include <source_location>

#include "macros.hpp"

namespace prelude {

// prints a message and aborts the program. To be used when a precondition was
// violated and there is nothing to be done.
[[noreturn]] void panic(
    const char* message,
    std::source_location sourceLocation = std::source_location::current());

// A version of `panic` which allows passing a string, which allows using
// `std::format`.
[[noreturn]] void panic(
    std::string message,
    std::source_location sourceLocation = std::source_location::current());

}  // namespace prelude

#endif  // PANIC_HPP
