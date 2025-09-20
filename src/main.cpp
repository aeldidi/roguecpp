#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include <string>

constexpr int screenWidth = 1280;
constexpr int screenHeight = 720;

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr;
SDL_Surface* helloWorld = nullptr;

bool init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

int main() {}
