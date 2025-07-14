#include "RenderingSystem.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
#include "Graphics/IRenderer.hpp"
#include "World/World.hpp"

namespace Crane::Systems {
  void RenderingSystem::Render(World::World &world, Graphics::IRenderer &renderer) {
    auto &registry = world.GetRegistry();
    auto view = registry.view<Components::Transform, Components::Renderable>();

    for (auto entity : view) {
      const auto &transform = view.get<Components::Transform>(entity);
      const auto &renderable = view.get<Components::Renderable>(entity);

      Graphics::Rect rect{transform.position.x - renderable.width / 2.0f, transform.position.y - renderable.height / 2.0f, renderable.width,
                          renderable.height};

      renderer.DrawRect(rect, renderable.color);
    }
  }
}