#pragma once

#include "Vertex.hpp"

namespace Crane::Graphics {
  struct Mesh {
    SVertex2List vertices;
    IndexList indices;
  };

  class MeshBuilder {
  public:
    MeshBuilder() = default;
    ~MeshBuilder() = default;

    inline static Mesh CreateQuad(const Math::Vec2f &size, const Color &color = Colors::White) {
      return {
          .vertices = {{{-size.x / 2, -size.y / 2}, color, {0.f, 0.f}},
                       {{size.x / 2, -size.y / 2}, color, {1.f, 0.f}},
                       {{size.x / 2, size.y / 2}, color, {1.f, 1.f}},
                       {{-size.x / 2, size.y / 2}, color, {0.f, 1.f}}},
          .indices = {{0, 1, 2, 2, 3, 0}}
      };
    }

    inline static Mesh CreateCircle(f32 radius, i32 segments, const Color &color = Colors::White) {
      SVertex2List vertices;
      constexpr f32 PI = 3.14159265359f;
      for (i32 i = 0; i < segments; ++i) {
        f32 angle = (2.0f * PI * i) / segments;
        f32 u = (cos(angle) + 1.0f) * 0.5f;
        f32 v = (sin(angle) + 1.0f) * 0.5f;
        vertices.push_back({
            {radius * cos(angle), radius * sin(angle)},
            color, {u,                   v                  }
        });
      }

      vertices.push_back({
          {0.0f, 0.0f},
          color, {0.5f, 0.5f}
      });

      IndexList indices;
      for (i32 i = 0; i < segments; ++i) {
        indices.push_back(i);
        indices.push_back((i + 1) % segments);
        indices.push_back(segments);
      }

      return {vertices, indices};
    }
  };
}