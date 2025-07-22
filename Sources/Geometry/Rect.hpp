#pragma once

#include "Core/Types.hpp"
#include <SDL3/SDL_rect.h>

namespace Crane::Geometry {
  struct Rect {
  public:
    Rect(f32 x = 0.0f, f32 y = 0.0f, f32 width = 0.0f, f32 height = 0.0f) : x(x), y(y), width(width), height(height) {}
    Rect(const SDL_FRect &rect) : x(rect.x), y(rect.y), width(rect.w), height(rect.h) {}

    operator SDL_FRect() const {
      return SDL_FRect{x, y, width, height};
    }

  public:
    f32 x, y;
    f32 width, height;
  };
}