#ifndef PRELUDE_THREADPOOL_HPP
#define PRELUDE_THREADPOOL_HPP

#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>

#include <string>
#include <variant>

#include "Error.hpp"
#include "Option.hpp"

namespace prelude {

struct CurrentThread {
  SDL_Thread* instance;
  // Returns the `SDL_Thread` of the current thread. This is `nullptr` if you
  // are on a thread which is not part of a threadpool.
  static SDL_Thread* get();

  // Returns true if called from the main thread.
  static bool isMainThread();

 private:
  CurrentThread();
};

// An error that can occur when calling `Threadpool::withThreads` or
// `Threadpool::create`.
enum CreateThreadpoolError {
  // `std::thread::hardware_concurrency()` returned 0.
  HardwareConcurrencyWas0,
  // some other error occurred. Check the error payload for a description.
  Other,
};

// A pool of threads the application manages.
// Uses a fixed size queue of the size passed when initialized.
struct Threadpool {
  using ThreadpoolError = Error<CreateThreadpoolError, std::string>;

  // A single job in the threadpool. Consists of an `SDL_ThreadFunction` and
  // a `void*` argument.
  struct Job {
    SDL_ThreadFunction func;
    void* arg;
  };

  // The max number of threads a threadpool can have.
  static constexpr int MaxThreads = 255;
  // The max number of jobs a threadpool can have.
  static constexpr int MaxJobs = 255;

  // creates a new `Threadpool` with the specified number of threads.
  // Returns `CreateThreadpoolError::Other` when it fails, and sets the payload
  // to a description of the error.
  static std::variant<Threadpool, ThreadpoolError> withThreads(
      uint8_t numThreads);

  // creates a new `Threadpool` with `std::thread::hardware_concurrency()`
  // number of threads.
  static std::variant<Threadpool, ThreadpoolError> create();

  // Pushes a new job onto the threadpool, optionally giving it a name.
  void push(SDL_ThreadFunction&& func, void*&& data,
            Option<std::string>&& name = Option<std::string>::None());

  ~Threadpool();
  // Remove default initialization
  Threadpool() = delete;
  // Remove copy constructor
  Threadpool(const Threadpool&) = delete;
  // Remove copy assignment
  Threadpool& operator=(const Threadpool&) = delete;
  // Allow move constructing
  Threadpool(Threadpool&&) = default;
  // Remove move assignment
  Threadpool& operator=(Threadpool&&) = delete;

 protected:
  Threadpool(uint8_t numThreads) : numThreads(numThreads) {}
  SDL_Thread* threads[MaxThreads] = {};
  Threadpool::Job jobs[MaxJobs] = {};
  SDL_Mutex* lock = SDL_CreateMutex();
  SDL_Condition* available = SDL_CreateCondition();
  uint8_t queueFront = 0;
  uint8_t queueBack = 0;
  uint8_t queueSize = 0;
  uint8_t queueCap = MaxJobs;
  uint8_t numThreads = 1;
  // Pops a new job from the queue, returning it.
  Job queuePop();
  friend int threadFunc(void* arg);
};

}  // namespace prelude

#endif  // PRELUDE_THREADPOOL_HPP
