#include "panic.hpp"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_thread.h>

#include <exception>
#include <format>
#include <source_location>

#include "Threadpool.hpp"

namespace prelude {

[[noreturn]] void panic(const char* message,
                        std::source_location sourceLocation) {
  // TODO: get the current thread, log a message then exit.
  auto thread = CurrentThread::get();
  std::string threadName = "<unknown>";
  if (CurrentThread::isMainThread()) {
    threadName = "<main>";
  } else if (thread != nullptr) {
    SDL_Thread* t = CurrentThread::get();
    threadName = std::format("'{}'", SDL_GetThreadName(t));
  }

  SDL_Log("thread %s panicked at %s:%u:%u in '%s' %s\n", threadName.c_str(),
          sourceLocation.file_name(), sourceLocation.line(),
          sourceLocation.column(), sourceLocation.function_name(), message);
  std::terminate();
}

[[noreturn]] void panic(std::string message,
                        std::source_location sourceLocation) {
  panic(message.c_str(), sourceLocation);
}

}  // namespace prelude
