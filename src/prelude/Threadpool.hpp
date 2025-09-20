#ifndef PRELUDE_THREADPOOL_HPP
#define PRELUDE_THREADPOOL_HPP

#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>

#include <string>

#include "Option.hpp"

namespace prelude {

// A pool of threads the application manages.
// Uses a fixed size queue of the size passed when initialized.
struct Threadpool {
  // A single job in the threadpool. Consists of an `SDL_ThreadFunction` and
  // a `void*` argument.
  struct Job {
    SDL_ThreadFunction func;
    void* arg;
  };

  // The max number of threads a threadpool can have.
  static constexpr int MaxThreads = 256;
  // The max number of jobs a threadpool can have.
  static constexpr int MaxJobs = 256;

  // creates a new `Threadpool` with the specified number of threads.
  static Threadpool withThreads(uint8_t numThreads);

  Threadpool() = delete;
  ~Threadpool();
  // Pushes a new job onto the threadpool, optionally giving it a name.
  void push(SDL_ThreadFunction&& func, void*&& data,
            Option<std::string>&& name = Option<std::string>::None());

 protected:
  SDL_Thread* threads[MaxThreads];
  Threadpool::Job jobs[MaxJobs];
  SDL_Mutex* lock;
  SDL_Condition* available;
  uint8_t queueFront;
  uint8_t queueBack;
  uint8_t queueSize;
  uint8_t queueCap;
  uint8_t numThreads;
  // Pops a new job from the queue, returning it.
  Job queuePop();
};

}  // namespace prelude

#endif  // PRELUDE_THREADPOOL_HPP
