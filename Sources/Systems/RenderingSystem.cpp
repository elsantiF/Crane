#include "RenderingSystem.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
#include "Core/Profiler.hpp"
#include "Graphics/IRenderer.hpp"
#include "World/World.hpp"

namespace Crane::Systems {
  void RenderingSystem::Render(World::World &world, Graphics::IRenderer &renderer) {
    PROFILE_SCOPE();
    auto &registry = world.GetRegistry();
    auto view = registry.view<Components::Transform, Components::Renderable>();

    for (auto entity : view) {
      const auto &[transform, renderable] = view.get<Components::Transform, Components::Renderable>(entity);

      Graphics::Rect rect{transform.transform.position.x - renderable.width / 2.0f, transform.transform.position.y - renderable.height / 2.0f,
                          renderable.width, renderable.height};

      renderer.SetFillColor(renderable.color);
      renderer.DrawRect(rect, transform.transform.rotation);
    }
  }
}