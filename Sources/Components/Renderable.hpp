#pragma once

#include "Core/Types.hpp"
#include "Graphics/Color.hpp"
#include "Math/Vector.hpp"

namespace Crane::Components {
  struct Renderable {
    Graphics::Color color;
    Vector<Math::Vec2f> vertices;
    Vector<i32> indices;
    i32 vertexCount = 0;
    i32 indexCount = 0;

    Renderable(Graphics::Color color = Graphics::Colors::White, Vector<Math::Vec2f> vertices = {}, Vector<i32> indices = {})
        : color(color), vertices(vertices), indices(indices), vertexCount(static_cast<i32>(vertices.size())),
          indexCount(static_cast<i32>(indices.size())) {}
  };
}