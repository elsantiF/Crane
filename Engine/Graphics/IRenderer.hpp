#pragma once

#include "Base/Math/Transform.hpp"
#include "Base/Math/Vector.hpp"
#include "Base/Types.hpp"
#include "Primitives/Color.hpp"
#include "Primitives/Mesh.hpp"
#include "Primitives/Texture.hpp"
#include "Resources/Resource.hpp"

namespace Crane::Graphics {
  class IRenderer {
  public:
    virtual ~IRenderer() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual void BeginImGuiFrame() = 0;
    virtual void EndImGuiFrame() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Clear(const Color &color) = 0;
    virtual void Present() = 0;

    virtual Id LoadMesh(const Mesh &mesh) = 0;
    virtual void UnloadMesh(Id meshId) = 0;

    virtual Id LoadTexture(Resources::Resource<Texture> textureResource) = 0;
    virtual void UnloadTexture(Id textureId) = 0;

    virtual void SetFillColor(const Color &color) = 0;
    virtual void SetMesh(Id meshId) = 0;
    virtual void SetTexture(Id textureId) = 0;

    virtual void DrawPoint(const Math::Vec2f &point) = 0;
    virtual void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) = 0;
    virtual void DrawRenderable(const Math::Transform &transform) = 0;

    virtual String GetName() const = 0;
  };
}