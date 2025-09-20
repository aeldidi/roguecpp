#ifndef PANIC_HPP
#define PANIC_HPP

#include <cstdlib>
#include <string_view>

namespace prelude {

// prints a message and aborts the program. To be used when a precondition was
// violated and there is nothing to be done.
[[noreturn]] void panic(std::string_view&& message);

}  // namespace prelude

#endif  // PANIC_HPP
