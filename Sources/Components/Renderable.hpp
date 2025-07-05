#pragma once

#include <SDL3/SDL_pixels.h>

struct Renderable {
  SDL_FColor color;
  float width, height;

  Renderable(SDL_FColor color = {255, 255, 255, 255}, float width = 32.0f,
             float height = 32.0f)
      : color(color), width(width), height(height) {}
};