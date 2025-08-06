#pragma once

#include "Base/Math/Vector.hpp"
#include "Color.hpp"

namespace Crane::Graphics {
  enum class VertexElementFormat { Float1, Float2, Float3, Float4, Int1, Int2, Int3, Int4 };

  struct VertexDefinition {
    u32 location;
    VertexElementFormat format;
    u32 offset;
  };

  /*
   * Simple 2D Vertex
   */
  struct SVertex2 {
    Math::Vec3f position;
    Color color;
    Math::Vec2f uv;

    SVertex2() = default;
    SVertex2(const Math::Vec3f &pos, const Color &col, const Math::Vec2f &texCoord) : position(pos), color(col), uv(texCoord) {}

    const Vector<VertexDefinition> GetDefinition() const {
      return {
          {0, VertexElementFormat::Float3, offsetof(SVertex2, position)},
          {1, VertexElementFormat::Float4, offsetof(SVertex2, color)   },
          {2, VertexElementFormat::Float2, offsetof(SVertex2, uv)      },
      };
    }

    const u32 GetSize() const {
      return sizeof(SVertex2);
    }
  };

  typedef Vector<SVertex2> SVertex2List;

  typedef Vector<u32> IndexList;
}