#pragma once

#include "Core/Types.hpp"
#include "Graphics/Color.hpp"
#include "Math/Vector.hpp"

namespace Crane::Components {
  struct Renderable {
    Graphics::Color color;
    Math::Vec2f size;

    Renderable(Graphics::Color color = Graphics::Colors::White, f32 width = 32.0f, f32 height = 32.0f) : color(color), size(width, height) {}
  };
}