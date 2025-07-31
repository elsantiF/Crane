#include "Application/Application.hpp"
#include "Base/Profiler.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/TextureManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "PlayerComponent.hpp"
#include "PlayerSystem.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/RigidBody.hpp"
#include "Scene/Components/Transform.hpp"
#include <imgui.h>
#include <iostream>
#include <numbers>

using namespace Crane;

Graphics::SVertex2List CreateSquareVertices(f32 xdim, f32 ydim, Graphics::Color color) {
  return {
      {{-xdim / 2, -ydim / 2}, color, {0.f, 0.f}},
      {{xdim / 2, -ydim / 2},  color, {1.f, 0.f}},
      {{xdim / 2, ydim / 2},   color, {1.f, 1.f}},
      {{-xdim / 2, ydim / 2},  color, {0.f, 1.f}}
  };
}

Vector<i32> CreateSquareIndices() {
  return {0, 1, 2, 2, 3, 0};
}

Graphics::SVertex2List CreateCircleVertices(f32 radius, i32 segments, Graphics::Color color) {
  Graphics::SVertex2List vertices;
  for (i32 i = 0; i < segments; ++i) {
    f32 angle = (2.0f * std::numbers::pi_v<f32> * i) / segments;
    f32 u = (cos(angle) + 1.0f) * 0.5f;
    f32 v = (sin(angle) + 1.0f) * 0.5f;
    vertices.push_back({
        {radius * cos(angle), radius * sin(angle)},
        color, {u,                   v                  }
    });
  }

  vertices.push_back({
      {0.0f, 0.0f},
      color, {0.5f, 0.5f}
  });
  return vertices;
}

Vector<i32> CreateCircleIndices(i32 segments) {
  Vector<i32> indices;
  for (i32 i = 0; i < segments; ++i) {
    indices.push_back(i);
    indices.push_back((i + 1) % segments);
    indices.push_back(segments);
  }
  return indices;
}

const ApplicationInfo appInfo = {
    "Crane Demo", {1,    0,   0   },
     {1600, 900, true}
};

static Graphics::TextureManager textureManager;

class Demo : public Application {
public:
  Demo() : Application(appInfo) {}

protected:
  void OnInitialize() override {
    // TODO: merge this two lines
    m_PhysicsSystem = GetWorld().AddSystem<Physics::PhysicsSystem>(GetWorld(), Math::Vec2f{0.0f, 9.81f}, 30.0f, 4);

    m_BoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::White));
    m_BoxIndexDataId = m_Renderer->LoadIndexData(CreateSquareIndices());
    m_CircleVertexDataId = m_Renderer->LoadVertexData(CreateCircleVertices(20.0f, 16, Graphics::Colors::White));
    m_CircleIndexDataId = m_Renderer->LoadIndexData(CreateCircleIndices(16));

    auto squareTexture = textureManager.LoadTexture("Resources/square.png");
    auto circleTexture = textureManager.LoadTexture("Resources/circle.png");

    m_SquareTextureId = m_Renderer->LoadTexture(*squareTexture);
    m_CircleTextureId = m_Renderer->LoadTexture(*circleTexture);

    // Create ground body
    Id groundVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(1000.0f, 50.0f, Graphics::Colors::Green));
    Scene::Entity ground = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(ground, Math::Vec2f{512.0f, 725.0f}, 0.1f);
      GetWorld().AddComponent<Scene::Components::Renderable>(ground, groundVertexDataId, m_BoxVertexDataId, 0);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {512,  700},
          {1000, 50 },
          Physics::BodyType::Static
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(ground, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(ground, boxcollider);
    }

    // Create a dynamic box body
    Id redBoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::Red));
    Scene::Entity box = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(box, Math::Vec2f{400.0f, 100.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(box, redBoxVertexDataId, m_BoxVertexDataId, 0);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {400, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(box, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(box, boxcollider);
    }

    Id blueBoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::Blue));
    m_Player = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(m_Player, Math::Vec2f{600.0f, 100.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(m_Player, blueBoxVertexDataId, m_BoxIndexDataId, 0);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {600, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(m_Player, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(m_Player, boxcollider);
      GetWorld().AddComponent<PlayerComponent>(m_Player);
    }

    GetWorld().AddSystem<PlayerSystem>(GetWorld(), *this, m_Player);
  }
  void OnPreFixedUpdate() override {}
  void OnPostFixedUpdate() override {}
  void OnPreUpdate() override {}
  void OnPostUpdate() override {}
  void OnPreRender() override {}
  void OnPostRender() override {}
  void OnImGui() override {
    PROFILE_SCOPE();
    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("Delta: %.3f ms", m_DeltaTime * 1000.0);
    ImGui::Text("ESC to exit");
    ImGui::End();

    auto &registry = m_World->GetRegistry();
    Editor::EntityDisplay::DrawEntityList(registry);

    ImGui::Begin("Demo Controls");

    if (ImGui::Button("Spawn Box")) {
      Scene::Entity box = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      GetWorld().AddComponent<Scene::Components::Transform>(box, Math::Vec2f{x, y});
      GetWorld().AddComponent<Scene::Components::Renderable>(box, m_BoxVertexDataId, m_BoxIndexDataId, m_SquareTextureId);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {x,  y },
          {40, 40},
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(box, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(box, boxcollider);
    }

    if (ImGui::Button("Spawn Circle")) {
      Scene::Entity circle = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      GetWorld().AddComponent<Scene::Components::Transform>(circle, Math::Vec2f{x, y});
      GetWorld().AddComponent<Scene::Components::Renderable>(circle, m_CircleVertexDataId, m_CircleIndexDataId, m_CircleTextureId);
      auto [rb, circlecollider] = m_PhysicsSystem->CreateCircleBody({
          {x, y},
          20, Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(circle, rb);
      GetWorld().AddComponent<Scene::Components::CircleCollider>(circle, circlecollider);
    }

    ImGui::End();
  }

private:
  Scene::Entity m_Player;
  Id m_BoxVertexDataId = 0;
  Id m_BoxIndexDataId = 0;
  Id m_CircleVertexDataId = 0;
  Id m_CircleIndexDataId = 0;
  Id m_SquareTextureId = 0;
  Id m_CircleTextureId = 0;
  Physics::PhysicsSystem *m_PhysicsSystem;
};

int main() {
  Demo demo;
  if (!demo.Initialize()) {
    std::cerr << "Failed to initialize Crane." << std::endl;
    return -1;
  }

  demo.Run();
  return 0;
}