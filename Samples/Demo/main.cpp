#include "Application/Application.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Physics/PhysicsFactory.hpp"
#include "World/Entity.hpp"
#include <imgui.h>
#include <iostream>

using namespace Crane;

class Demo : public Core::Application {
protected:
  void OnInitialize() override {
    auto &physicsWorld = GetWorld().GetPhysicsWorld();
    auto ppm = GetWorld().GetPixelsPerMeter();

    // Create ground body
    World::Entity ground = GetWorld().CreateEntity();
    {
      ground.AddComponent<Components::Transform>(Math::Vec2f{512.0f, 725.0f}, 0.1f);
      ground.AddComponent<Components::Renderable>(Graphics::Color{0, 255, 0, 255}, 1000.0f, 50.0f);

      auto [rb, boxcollider] = Physics::PhysicsFactory::CreateBoxBody(physicsWorld, {512, 700, 1000, 50, Physics::BodyType::Static}, ppm);
      ground.AddComponent<Components::Rigidbody>(rb);
      ground.AddComponent<Components::BoxCollider>(boxcollider);
    }

    // Create a dynamic box body
    World::Entity box = GetWorld().CreateEntity();
    {
      box.AddComponent<Components::Transform>(Math::Vec2f{400.0f, 100.0f});
      box.AddComponent<Components::Renderable>(Graphics::Color{255, 0, 0, 255}, 40.0f, 40.0f);

      auto [rb, boxcollider] = Physics::PhysicsFactory::CreateBoxBody(physicsWorld, {400, 100, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::Rigidbody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    World::Entity blueBox = GetWorld().CreateEntity();
    {
      blueBox.AddComponent<Components::Transform>(Math::Vec2f{600.0f, 100.0f});
      blueBox.AddComponent<Components::Renderable>(Graphics::Color{0, 0, 255, 255}, 40.0f, 40.0f);
    }
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
      auto [rb, boxcollider] = Physics::PhysicsFactory::CreateBoxBody(physicsWorld, {x, y, 40, 40, Physics::BodyType::Dynamic}, ppm);
      box.AddComponent<Components::Rigidbody>(rb);
      box.AddComponent<Components::BoxCollider>(boxcollider);
    }

    ImGui::End();
  }
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