#include "Application/Application.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "PlayerComponent.hpp"
#include "PlayerSystem.hpp"
#include "World/Entity.hpp"
#include <imgui.h>
#include <iostream>
#include <numbers>

using namespace Crane;

Vector<Math::Vec2f> CreateSquareVertices(f32 xdim, f32 ydim) {
  return {
      {-xdim / 2, -ydim / 2},
      {xdim / 2,  -ydim / 2},
      {xdim / 2,  ydim / 2 },
      {-xdim / 2, ydim / 2 }
  };
}

Vector<i32> CreateSquareIndices() {
  return {0, 1, 2, 2, 3, 0};
}

Vector<Math::Vec2f> CreateCircleVertices(f32 radius, i32 segments) {
  Vector<Math::Vec2f> vertices;
  for (i32 i = 0; i < segments; ++i) {
    f32 angle = (2.0f * std::numbers::pi_v<f32> * i) / segments;
    vertices.push_back({radius * cos(angle), radius * sin(angle)});
  }

  vertices.push_back({0.0f, 0.0f});
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

class Demo : public Application::Application {
public:
  Demo() = default;

protected:
  void OnInitialize() override {
    auto &physicsWorld = GetWorld().GetPhysicsWorld();
    auto ppm = GetWorld().GetPixelsPerMeter();

    // Create ground body
    World::Entity ground = GetWorld().CreateEntity();
    {
      ground.AddComponent<Components::Transform>(Math::Vec2f{512.0f, 725.0f}, 0.1f);
      ground.AddComponent<Components::Renderable>(Graphics::Colors::Green, CreateSquareVertices(1000.0f, 50.0f), CreateSquareIndices());

      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({512, 700, 1000, 50, Physics::BodyType::Static}, ppm);
      ground.AddComponent<Components::RigidBody>(rb);
      ground.AddComponent<Components::BoxCollider>(boxcollider);
    }

    // Create a dynamic box body
    World::Entity box = GetWorld().CreateEntity();
    {
      box.AddComponent<Components::Transform>(Math::Vec2f{400.0f, 100.0f});
      box.AddComponent<Components::Renderable>(Graphics::Colors::Red, CreateSquareVertices(40.0f, 40.0f), CreateSquareIndices());

      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({400, 100, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::RigidBody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    m_Player = GetWorld().CreateEntity();
    {
      m_Player.AddComponent<Components::Transform>(Math::Vec2f{600.0f, 100.0f});
      m_Player.AddComponent<Components::Renderable>(Graphics::Colors::Blue, CreateSquareVertices(40.0f, 40.0f), CreateSquareIndices());
      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({600, 100, 40, 40, Physics::BodyType::Dynamic}, ppm);
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
      auto &physicsWorld = GetWorld().GetPhysicsWorld();
      auto ppm = GetWorld().GetPixelsPerMeter();
      World::Entity box = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      box.AddComponent<Components::Transform>(Math::Vec2f{x, y});
      box.AddComponent<Components::Renderable>(Graphics::Colors::Yellow, CreateSquareVertices(40.0f, 40.0f), CreateSquareIndices());
      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({x, y, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::RigidBody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    if (ImGui::Button("Spawn Circle")) {
      auto &physicsWorld = GetWorld().GetPhysicsWorld();
      auto ppm = GetWorld().GetPixelsPerMeter();
      World::Entity circle = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      circle.AddComponent<Components::Transform>(Math::Vec2f{x, y});
      circle.AddComponent<Components::Renderable>(Graphics::Colors::White, CreateCircleVertices(20.0f, 16), CreateCircleIndices(16));
      auto [rb, circlecollider] = physicsWorld.CreateCircleBody({x, y, 20, Physics::BodyType::Dynamic}, ppm);
      circle.AddComponent<Components::RigidBody>(rb);
      circle.AddComponent<Components::CircleCollider>(circlecollider);
    }

    ImGui::End();
  }

private:
  World::Entity m_Player;
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