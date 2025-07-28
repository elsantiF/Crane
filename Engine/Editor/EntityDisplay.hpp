#pragma once

#include <entt/entt.hpp>

namespace Crane::Editor {
  class EntityDisplay {
  public:
    static void DrawEntityList(entt::registry &registry);

  private:
    static void DrawEntity(entt::registry &registry, entt::entity entity);
  };
}