#include "Game.hpp"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

Uint64 frameStartNs = 0;
constexpr int TargetFps = 60;
constexpr Uint64 NanosecondsPerSecond = 1000000000;
constexpr Uint64 FrameDelayNs = NanosecondsPerSecond / TargetFps;

bool Game::tick() {
  frameStartNs = SDL_GetTicksNS();
  if (!this->shouldContinue) {
    return false;
  }

  SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
  SDL_RenderClear(this->renderer);
  this->monky.render(this->renderer, 0.0, 0.0);
  SDL_RenderPresent(this->renderer);

  Uint64 elapsedNs = SDL_GetTicksNS() - frameStartNs;
  if (elapsedNs < FrameDelayNs) {
    SDL_DelayNS(FrameDelayNs - elapsedNs);
  }
  Uint64 frameTime = SDL_GetTicksNS() - frameStartNs;
  SDL_Log("Frame Time (ms): %lu\n", frameTime / 1000000);
  SDL_Log("FPS: %lu\n", frameTime / NanosecondsPerSecond);
  return true;
}

void Game::requestQuit() { this->shouldContinue = false; }

void Game::onResize(Sint32 newWidth, Sint32 newHeight) {
  auto width = uint32_t{0};
  if (newWidth > 0) {
    width = newWidth;
  }

  auto height = uint32_t{0};
  if (newHeight > 0) {
    height = newHeight;
  }

  this->renderWidth = width;
  this->renderHeight = height;
  this->screenSurface = SDL_GetWindowSurface(this->window);
}
