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

using namespace Crane;

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
      ground.AddComponent<Components::Renderable>(Graphics::Color{0, 255, 0, 255}, 1000.0f, 50.0f);

      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({512, 700, 1000, 50, Physics::BodyType::Static}, ppm);
      ground.AddComponent<Components::Rigidbody>(rb);
      ground.AddComponent<Components::BoxCollider>(boxcollider);
    }

    // Create a dynamic box body
    World::Entity box = GetWorld().CreateEntity();
    {
      box.AddComponent<Components::Transform>(Math::Vec2f{400.0f, 100.0f});
      box.AddComponent<Components::Renderable>(Graphics::Color{255, 0, 0, 255}, 40.0f, 40.0f);

      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({400, 100, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::Rigidbody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    m_Player = GetWorld().CreateEntity();
    {
      m_Player.AddComponent<Components::Transform>(Math::Vec2f{600.0f, 100.0f});
      m_Player.AddComponent<Components::Renderable>(Graphics::Color{0, 0, 255, 255}, 40.0f, 40.0f);
      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({600, 100, 40, 40, Physics::BodyType::Dynamic}, ppm);
      m_Player.AddComponent<Components::Rigidbody>(rb);
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
      box.AddComponent<Components::Renderable>(Graphics::Color{255, 0, 0, 255}, 40.0f, 40.0f);
      auto [rb, boxcollider] = physicsWorld.CreateBoxBody({x, y, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::Rigidbody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
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