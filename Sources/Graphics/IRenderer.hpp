#pragma once

#include "Color.hpp"
#include "Core/Types.hpp"
#include "Math/Transform.hpp"
#include "Math/Vector.hpp"
#include "Vertex.hpp"

namespace Crane::Graphics {
  class IRenderer {
  public:
    virtual ~IRenderer() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void InitializeImGui() = 0;
    virtual void ShutdownImGui() = 0;
    virtual void BeginImGuiFrame() = 0;
    virtual void EndImGuiFrame() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Clear(const Color &color) = 0;
    virtual void Present() = 0;

    virtual u32 LoadVertexData(const SVertex2List &vertices) = 0;
    virtual void UnloadVertexData(u32 vertexDataId) = 0;

    virtual u32 LoadIndexData(const IndexList &indices) = 0;
    virtual void UnloadIndexData(u32 indexDataId) = 0;

    virtual void SetFillColor(const Color &color) = 0;
    virtual void SetVertexData(u32 vertexDataId) = 0;
    virtual void SetIndexData(u32 indexDataId) = 0;

    virtual void DrawPoint(const Math::Vec2f &point) = 0;
    virtual void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) = 0;
    virtual void DrawRenderable(const Math::Transform &transform) = 0;

    virtual String GetName() const = 0;
  };
}