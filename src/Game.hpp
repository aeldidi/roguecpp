#ifndef GAME_HPP
#define GAME_HPP

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include "Texture.hpp"
#include "prelude/macros.hpp"

constexpr int DefaultScreenWidth = 1280;
constexpr int DefaultScreenHeight = 720;

struct Game {
  SDL_Window* window = nullptr;
  SDL_Surface* screenSurface = nullptr;
  Texture monky;
  SDL_Renderer* renderer = nullptr;
  bool shouldContinue = true;
  uint32_t renderWidth = DefaultScreenWidth;
  uint32_t renderHeight = DefaultScreenHeight;

  // Runs a single frame of the game. Should return true or false if the game
  // should continue running or not.
  bool tick();

  // Requests that the game quit on the next tick.
  void requestQuit();

  // a callback to run when the window is resized.
  void onResize(Sint32 newWidth, Sint32 newHeight);

  Game(SDL_Window* w, Texture&& monky, SDL_Renderer* r)
      : window(w), monky(MOV(monky)), renderer(r) {
    this->screenSurface = SDL_GetWindowSurface(window);
  }
};

#endif  // GAME_HPP
