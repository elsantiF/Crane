#include "EntityDisplay.hpp"
#include "Components/Renderable.hpp"
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
        bool positionChanged = ImGui::DragFloat2("Position", &transform.position.x, 0.1f);
        bool rotationChanged = ImGui::DragFloat("Rotation", &transform.rotation, 0.1f);
        bool scaleChanged = ImGui::DragFloat2("Scale", &transform.scale.x, 0.1f);

        if (positionChanged || rotationChanged || scaleChanged) {
          transform.dirty = true;
        }
      }
    }

    if (registry.any_of<Components::Renderable>(entity)) {
      if (ImGui::CollapsingHeader("Renderable")) {
        auto &renderable = registry.get<Components::Renderable>(entity);
        ImGui::ColorEdit4("Color", &renderable.color.r);
        ImGui::DragFloat("Width", &renderable.width, 0.1f);
        ImGui::DragFloat("Height", &renderable.height, 0.1f);
      }
    }

    ImGui::PopID();
  }
}
