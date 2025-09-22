#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>

#include <cassert>
#include <variant>

#include "Game.hpp"
#include "Texture.hpp"

extern "C" {
SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto winProps = SDL_CreateProperties();
  SDL_SetStringProperty(winProps, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "game");
  SDL_SetNumberProperty(winProps, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,
                        DefaultScreenWidth);
  SDL_SetNumberProperty(winProps, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER,
                        DefaultScreenHeight);
  SDL_SetBooleanProperty(winProps, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN,
                         true);
  SDL_SetBooleanProperty(winProps, SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, true);
  auto window = SDL_CreateWindowWithProperties(winProps);
  if (window == nullptr) {
    SDL_Log("Window could not be created: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto renderProps = SDL_CreateProperties();
  SDL_SetPointerProperty(renderProps, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER,
                         window);
  auto renderer = SDL_CreateRendererWithProperties(renderProps);
  if (renderer == nullptr) {
    SDL_Log("Renderer could not be created: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  if (!SDL_SetRenderVSync(renderer, 1)) {
    SDL_Log("Couldn't enable VSync: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto monkyRes = Texture::loadFromFile(renderer, "assets/monky.jpg");
  if (std::holds_alternative<std::string>(monkyRes)) {
    SDL_Log("failed to load 'assets/monky.jpg': %s\n",
            std::get<std::string>(monkyRes).c_str());
    return SDL_APP_FAILURE;
  }
  auto& monky = std::get<Texture>(monkyRes);

  SDL_ShowWindow(window);
  *appstate = new Game(window, MOV(monky), renderer);
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  Game* game = static_cast<Game*>(appstate);
  assert(game != nullptr);

  bool shouldContinue = game->tick();
  if (!shouldContinue) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  Game* game = static_cast<Game*>(appstate);
  assert(game != nullptr);

  switch (event->type) {
    case SDL_EVENT_QUIT:
      game->requestQuit();
      break;
    case SDL_EVENT_WINDOW_RESIZED:
      game->onResize(event->window.data1, event->window.data2);
      break;
    default:
      break;
  }
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
  (void)result;
  Game* game = static_cast<Game*>(appstate);
  delete game;

  SDL_Quit();
}
}  // extern "C"
