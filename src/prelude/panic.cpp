#include "panic.hpp"

#include <exception>

namespace prelude {

[[noreturn]] void panic(std::string_view&& message) {
  // TODO: get the current thread, log a message then exit.
  std::terminate();
}

}  // namespace prelude
