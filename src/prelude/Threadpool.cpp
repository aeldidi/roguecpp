#include "Threadpool.hpp"

#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>

#include <cassert>
#include <mutex>
#include <string>
#include <thread>

#include "Option.hpp"
#include "panic.hpp"

namespace prelude {

SDL_TLSID currentThreadTlsId;
std::once_flag mainThreadIdOnceFlag;
SDL_ThreadID mainThreadId;

SDL_Thread* CurrentThread::get() {
  static CurrentThread instance;
  return static_cast<SDL_Thread*>(SDL_GetTLS(&currentThreadTlsId));
}

bool CurrentThread::isMainThread() {
  return SDL_GetCurrentThreadID() == mainThreadId;
}

CurrentThread::CurrentThread() {
  std::call_once(mainThreadIdOnceFlag,
                 [] { mainThreadId = SDL_GetCurrentThreadID(); });
}

struct WorkFunctionData {
  Threadpool* tp;
  SDL_Thread* current;
};

int threadFunc(void* arg) {
  auto* data = static_cast<WorkFunctionData*>(arg);
  assert(data != nullptr);
  assert(data->tp != nullptr);
  assert(data->current != nullptr);
  SDL_SetTLS(&currentThreadTlsId, data->current, nullptr);
  for (;;) {
    auto job = data->tp->queuePop();
    job.func(job.arg);
  }
}

Threadpool::Job Threadpool::queuePop() {
  SDL_LockMutex(this->lock);
  while (this->queueSize == 0) {
    SDL_WaitCondition(this->available, this->lock);
  }
  Threadpool::Job front = this->jobs[this->queueFront];
  this->queueFront = (this->queueFront + 1) % this->queueCap;
  this->queueSize -= 1;
  SDL_UnlockMutex(this->lock);
  return front;
}

Threadpool::~Threadpool() {
  SDL_LockMutex(this->lock);
  for (int i = 0; i < this->numThreads; i += 1) {
    SDL_WaitThread(this->threads[i], nullptr);
  }
  SDL_UnlockMutex(this->lock);

  SDL_DestroyMutex(this->lock);
  SDL_DestroyCondition(this->available);
}

std::variant<Threadpool, Threadpool::ThreadpoolError> Threadpool::withThreads(
    uint8_t numThreads) {
  auto result = Threadpool(numThreads);
  for (int i = 0; i < result.numThreads; i += 1) {
    auto tmp = SDL_CreateThread(threadFunc, "", nullptr);
    if (tmp == nullptr) {
      return ThreadpoolError{.errorCode = CreateThreadpoolError::Other,
                             .payload = SDL_GetError()};
    }
    result.threads[i] = tmp;
  }

  return result;
}

std::variant<Threadpool, Threadpool::ThreadpoolError> Threadpool::create() {
  unsigned int n = std::thread::hardware_concurrency();
  if (n == 0) {
    return ThreadpoolError{.errorCode =
                               CreateThreadpoolError::HardwareConcurrencyWas0};
  }

  return Threadpool::withThreads(n);
}

void Threadpool::push(SDL_ThreadFunction&& func, void*&& data,
                      Option<std::string>&& name) {
  SDL_LockMutex(this->lock);
  if (this->queueSize == this->queueCap) {
    panic("tried to push to full thread queue");
  }
  this->queueBack = (this->queueBack + 1) % this->queueCap;
  this->jobs[this->queueBack] = Threadpool::Job{func, data};
  this->queueSize += 1;
  SDL_BroadcastCondition(this->available);
  SDL_UnlockMutex(this->lock);
}

}  // namespace prelude
