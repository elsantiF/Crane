#pragma once

#include "Color.hpp"
#include "Core/Types.hpp"
#include "Rect.hpp"

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

    /*
     * Draw a rectangle with the given parameters.
     * The rectangle is defined by the Rect object, and the color is applied to it.
     * The rotation is in radians
     */
    virtual void DrawRect(const Rect &rect, const Color &color, f32 rotation) = 0;

    virtual String GetName() const = 0;
  };
}