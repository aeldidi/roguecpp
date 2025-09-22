#include "Texture.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3_image/SDL_image.h>

#include <cstdint>
#include <variant>

#include "SDL3/SDL_render.h"

std::variant<Texture, std::string> Texture::loadFromFile(SDL_Renderer* r,
                                                         const char* path) {
  auto tex = IMG_LoadTexture(r, path);
  if (tex == nullptr) {
    return SDL_GetError();
  }

  auto width = uint16_t{0};
  if (tex->w > 0) {
    width = tex->w;
  }
  auto height = uint16_t{0};
  if (tex->h > 0) {
    height = tex->h;
  }

  return Texture(tex, width, height, path);
}

Texture::~Texture() { SDL_DestroyTexture(this->texture); }

void Texture::render(SDL_Renderer* r, const float x, const float y) const {
  SDL_FRect dest = {
      .x = x,
      .y = y,
      .w = static_cast<float>(this->width),
      .h = static_cast<float>(this->height),
  };
  if (!SDL_RenderTexture(r, this->texture, nullptr, &dest)) {
    SDL_Log("error rendering texture '%s': %s\n", this->_fileName.c_str(),
            SDL_GetError());
  }
}

std::string Texture::fileName() const { return this->_fileName; }

bool Texture::isLoaded() const { return this->texture != nullptr; }
