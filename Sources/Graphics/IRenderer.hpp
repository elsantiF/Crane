#pragma once

#include "Color.hpp"
#include "Core/Types.hpp"
#include "Math/Vector.hpp"
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

    virtual void SetFillColor(const Color &color) = 0;

    virtual void DrawPoint(const Math::Vec2f &point) = 0;
    virtual void DrawLine(const Math::Vec2f &start, const Math::Vec2f &end) = 0;

    /*
     * Draw a rectangle with the given parameters.
     * The rectangle is defined by the Rect object, and the color is applied to it.
     * The rotation is in radians
     */
    virtual void DrawRect(const Rect &rect, f32 rotation) = 0;

    /*
     * Draw a circle with the given center, radius, and color.
     * The circle is filled with the specified color.
     */
    virtual void DrawCircle(const Math::Vec2f &center, f32 radius) = 0;

    virtual String GetName() const = 0;
  };
}