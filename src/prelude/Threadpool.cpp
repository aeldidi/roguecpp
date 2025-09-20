#include "Threadpool.hpp"

#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_thread.h>

#include <string>

#include "Option.hpp"
#include "panic.hpp"

namespace prelude {

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

static Threadpool withThreads(uint8_t numThreads) { return Threadpool() }

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

void threadFunc(void* arg) {}

}  // namespace prelude
