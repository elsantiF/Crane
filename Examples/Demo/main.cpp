#include "Application/Application.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "PlayerComponent.hpp"
#include "PlayerSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "World/Entity.hpp"
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

Graphics::Texture CreateSimpleTexture() {
  // Representation in a box:
  // 1 2
  // 3 4
  return {
      2,
      2,
      {
        0xFF0000FF, // First pixel
          0xFF00FF00, // Second pixel
          0xFFFF0000, // Third pixel
          0xFFFFFFFF  // Fourth pixel
      }
  };
}

class Demo : public Application::Application {
public:
  Demo() = default;

protected:
  void OnInitialize() override {
    m_PhysicsSystem = GetWorld().GetSystemManager().GetSystem<Systems::PhysicsSystem>();

    m_BoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::White));
    m_BoxIndexDataId = m_Renderer->LoadIndexData(CreateSquareIndices());
    m_CircleVertexDataId = m_Renderer->LoadVertexData(CreateCircleVertices(20.0f, 16, Graphics::Colors::White));
    m_CircleIndexDataId = m_Renderer->LoadIndexData(CreateCircleIndices(16));
    m_SimpleTextureId = m_Renderer->LoadTexture(CreateSimpleTexture());

    // Create ground body
    Id groundVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(1000.0f, 50.0f, Graphics::Colors::Green));
    World::Entity ground = GetWorld().CreateEntity();
    {
      ground.AddComponent<Components::Transform>(Math::Vec2f{512.0f, 725.0f}, 0.1f);
      ground.AddComponent<Components::Renderable>(groundVertexDataId, m_BoxVertexDataId, 0);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {512,  700},
          {1000, 50 },
          Physics::BodyType::Static
      });
      ground.AddComponent<Components::RigidBody>(rb);
      ground.AddComponent<Components::BoxCollider>(boxcollider);
    }

    // Create a dynamic box body
    Id redBoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::Red));
    World::Entity box = GetWorld().CreateEntity();
    {
      box.AddComponent<Components::Transform>(Math::Vec2f{400.0f, 100.0f});
      box.AddComponent<Components::Renderable>(redBoxVertexDataId, m_BoxVertexDataId, 0);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {400, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      box.AddComponent<Components::RigidBody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    Id blueBoxVertexDataId = m_Renderer->LoadVertexData(CreateSquareVertices(40.0f, 40.0f, Graphics::Colors::Blue));
    m_Player = GetWorld().CreateEntity();
    {
      m_Player.AddComponent<Components::Transform>(Math::Vec2f{600.0f, 100.0f});
      m_Player.AddComponent<Components::Renderable>(blueBoxVertexDataId, m_BoxIndexDataId, 0);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {600, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      m_Player.AddComponent<Components::RigidBody>(rb);
      m_Player.AddComponent<Components::BoxCollider>(boxcollider);
      m_Player.AddComponent<PlayerComponent>();
    }

    GetWorld().GetSystemManager().AddSystem<PlayerSystem>(*this, m_Player);
  }
  void OnPreFixedUpdate() override {}
  void OnPostFixedUpdate() override {}
  void OnPreUpdate() override {}
  void OnPostUpdate() override {}
  void OnPreRender() override {}
  void OnPostRender() override {}
  void OnImGui() override {
    ImGui::Begin("Demo Controls");

    if (ImGui::Button("Spawn Box")) {
      World::Entity box = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      box.AddComponent<Components::Transform>(Math::Vec2f{x, y});
      box.AddComponent<Components::Renderable>(m_BoxVertexDataId, m_BoxIndexDataId, m_SimpleTextureId);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {x,  y },
          {40, 40},
          Physics::BodyType::Dynamic
      });
      box.AddComponent<Components::RigidBody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    if (ImGui::Button("Spawn Circle")) {
      World::Entity circle = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      circle.AddComponent<Components::Transform>(Math::Vec2f{x, y});
      circle.AddComponent<Components::Renderable>(m_CircleVertexDataId, m_CircleIndexDataId, m_SimpleTextureId);
      auto [rb, circlecollider] = m_PhysicsSystem->CreateCircleBody({
          {x, y},
          20, Physics::BodyType::Dynamic
      });
      circle.AddComponent<Components::RigidBody>(rb);
      circle.AddComponent<Components::CircleCollider>(circlecollider);
    }

    ImGui::End();
  }

private:
  World::Entity m_Player;
  Id m_BoxVertexDataId = 0;
  Id m_BoxIndexDataId = 0;
  Id m_CircleVertexDataId = 0;
  Id m_CircleIndexDataId = 0;
  Id m_SimpleTextureId = 0;
  Systems::PhysicsSystem *m_PhysicsSystem;
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