#pragma once

#include "Core/Types.hpp"
#include <SDL3/SDL_render.h>

namespace Crane::Graphics {
  class Color {
  public:
    Color() : r(0), g(0), b(0), a(255) {}
    Color(f32 r, f32 g, f32 b, f32 a = 1.0) : r(r), g(g), b(b), a(a) {}
    Color(const SDL_FColor &color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

    operator SDL_FColor() const { return SDL_FColor{r, g, b, a}; }

  public:
    f32 r, g, b, a;
  };
}