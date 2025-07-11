#include "EntityDisplay.hpp"
#include "Components/Transform.hpp"
#include "Core/Types.hpp"
#include <imgui.h>

namespace Crane::Editor {
  void EntityDisplay::DrawEntityList(entt::registry &registry) {
    ImGui::Begin("Entities");

    for (auto entity : registry.view<entt::entity>()) {
      DrawEntity(registry, entity);
    }

    ImGui::End();
  }

  void EntityDisplay::DrawEntity(entt::registry &registry, entt::entity entity) {
    ImGui::PushID(static_cast<i32>(entity));
    ImGui::Text("Entity ID: %d", static_cast<i32>(entity));

    if (registry.any_of<Components::Transform>(entity)) {
      if (ImGui::CollapsingHeader("Transform")) {
        auto &transform = registry.get<Components::Transform>(entity);
        bool positionChanged = ImGui::DragFloat2("Position", &transform.x, 0.1f);
        bool rotationChanged = ImGui::DragFloat("Rotation", &transform.rotation, 0.1f);
        bool scaleChanged = ImGui::DragFloat2("Scale", &transform.scaleX, 0.1f);

        if (positionChanged || rotationChanged || scaleChanged) {
          transform.dirty = true;
        }
      }
    }

    ImGui::PopID();
  }
}
