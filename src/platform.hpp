#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL3/SDL_video.h>

namespace platform {

struct Window {
  SDL_Window* window = nullptr;
  SDL_Surface* screenSurface = nullptr;

  Window() = delete;
  // static std::optional<typename Tp>
};

}  // namespace platform

#endif  // WINDOW_HPP
