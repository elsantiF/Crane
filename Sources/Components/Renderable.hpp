#pragma once

#include "Core/Types.hpp"
#include <SDL3/SDL_pixels.h>

struct Renderable {
  SDL_FColor color;
  f32 width, height;

  Renderable(SDL_FColor color = {255, 255, 255, 255}, f32 width = 32.0f, f32 height = 32.0f) : color(color), width(width), height(height) {}
};