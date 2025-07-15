#include "EntityDisplay.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
#include "Core/Profiler.hpp"
#include "Core/Types.hpp"
#include <imgui.h>

namespace Crane::Editor {
  void EntityDisplay::DrawEntityList(entt::registry &registry) {
    PROFILE_SCOPE();
    ImGui::Begin("Entities");

    for (auto entity : registry.view<entt::entity>()) {
      DrawEntity(registry, entity);
    }

    ImGui::End();
  }

  void EntityDisplay::DrawEntity(entt::registry &registry, entt::entity entity) {
    PROFILE_SCOPE();
    ImGui::PushID(static_cast<i32>(entity));
    ImGui::Text("Entity ID: %d", static_cast<i32>(entity));

    if (auto *transform = registry.try_get<Components::Transform>(entity)) {
      if (ImGui::CollapsingHeader("Transform")) {
        bool positionChanged = ImGui::DragFloat2("Position", &transform->position.x, 0.1f);
        bool rotationChanged = ImGui::DragFloat("Rotation", &transform->rotation, 0.1f);

        if (positionChanged || rotationChanged) {
          transform->dirty = true;
        }
      }
    }

    if (auto *renderable = registry.try_get<Components::Renderable>(entity)) {
      if (ImGui::CollapsingHeader("Renderable")) {
        ImGui::ColorEdit4("Color", &renderable->color.r);
        ImGui::DragFloat("Width", &renderable->width, 0.1f);
        ImGui::DragFloat("Height", &renderable->height, 0.1f);
      }
    }

    if (auto *boxCollider = registry.try_get<Components::BoxCollider>(entity)) {
      if (ImGui::CollapsingHeader("Box Collider")) {
        bool changed = ImGui::DragFloat2("Dimensions", &boxCollider->dimensions.x, 0.1f);
        if (changed) {
          boxCollider->dirty = true;
        }
      }
    }

    ImGui::PopID();
  }
}
