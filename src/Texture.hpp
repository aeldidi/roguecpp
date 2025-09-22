#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <SDL3/SDL_render.h>

#include <cstdint>
#include <string>
#include <variant>

struct Texture {
  const uint16_t width;
  const uint16_t height;

  // Returns the texture's filename.
  std::string fileName() const;

  // Loads a texture from a file.
  static std::variant<Texture, std::string> loadFromFile(SDL_Renderer* r,
                                                         const char* path);

  // renders the texture at the given coordinates.
  void render(SDL_Renderer* r, const float x, const float y) const;

  // returns whether the texture is loaded or not.
  bool isLoaded() const;

  ~Texture();
  // Remove default initialization
  Texture() = delete;
  // Remove copy constructor
  Texture(const Texture&) = delete;
  // Remove copy assignment
  Texture& operator=(const Texture&) = delete;
  // Allow move constructing
  Texture(Texture&& other) noexcept
      : width(other.width),
        height(other.height),
        _fileName(other._fileName),
        texture(other.texture) {
    other.texture = nullptr;
  }
  // Remove move assignment
  Texture& operator=(Texture&&) = delete;

 private:
  std::string _fileName;
  SDL_Texture* texture = nullptr;
  Texture(SDL_Texture* tex, const uint16_t width, const uint16_t height,
          std::string fileName)
      : texture(tex), width(width), height(height), _fileName(fileName) {}
};

#endif  // TEXTURE_HPP
