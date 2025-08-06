#include "EntityDisplay.hpp"
#include "Base/Profiler.hpp"
#include "Base/Types.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include <imgui.h>
#include <numbers>

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

    if (auto *transform = registry.try_get<Scene::Components::Transform>(entity)) {
      if (ImGui::CollapsingHeader("Transform")) {
        bool positionChanged = ImGui::DragFloat2("Position", &transform->transform.position.x, 0.1f);
        bool rotationChanged = ImGui::DragFloat("Rotation", &transform->transform.rotation, 0.05f, 0.0f, std::numbers::pi_v<float> * 2.0f);

        if (positionChanged || rotationChanged) {
          transform->dirty = true;
        }
      }
    }

    if (auto *renderable = registry.try_get<Scene::Components::Renderable>(entity)) {
      if (ImGui::CollapsingHeader("Renderable")) {
        ImGui::Text("Vertex Buffer ID: %u", renderable->vertexBufferId);
        ImGui::Text("Index Buffer ID: %u", renderable->indexBufferId);
        ImGui::Text("Texture ID: %u", renderable->textureId);
      }
    }

    if (auto *boxCollider = registry.try_get<Scene::Components::BoxCollider>(entity)) {
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
