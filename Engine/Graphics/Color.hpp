#pragma once

#include "Base/Types.hpp"
#include <SDL3/SDL_render.h>

namespace Crane::Graphics {

  class Color {
  public:
    Color() : r(0.f), g(0.f), b(0.f), a(1.f) {}
    Color(f32 r, f32 g, f32 b, f32 a = 1.0) : r(r), g(g), b(b), a(a) {}
    Color(const SDL_FColor &color) : r(color.r), g(color.g), b(color.b), a(color.a) {}

    operator SDL_FColor() const {
      return SDL_FColor{r, g, b, a};
    }

    bool operator==(const Color &other) const {
      return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const Color &other) const {
      return !(*this == other);
    }

  public:
    f32 r, g, b, a;
  };

  namespace Colors {
    inline static const Color White = Color{1.f, 1.f, 1.f, 1.f};
    inline static const Color Black = Color{0.f, 0.f, 0.f, 1.f};
    inline static const Color Red = Color{1.f, 0.f, 0.f, 1.f};
    inline static const Color Green = Color{0.f, 1.f, 0.f, 1.f};
    inline static const Color Blue = Color{0.f, 0.f, 1.f, 1.f};
    inline static const Color Yellow = Color{1.f, 1.f, 0.f, 1.f};
    inline static const Color CLEAR_COLOR = {0.05f, 0.05f, 0.05f, 1.0f};
  }
}