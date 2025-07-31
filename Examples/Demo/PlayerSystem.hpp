#pragma once

#include "Application/Application.hpp"
#include "Core/Systems/ISystem.hpp"
#include "Events/Events.hpp"
#include "PlayerComponent.hpp"

class PlayerSystem : public Crane::Core::Systems::IUpdateSystem {
public:
  PlayerSystem(Crane::Core::Application &app, Crane::Scene::Entity &playerEntity) : m_PlayerEntity(&playerEntity), m_App(&app) {}

  void Initialize(Crane::Scene::World &world) override;

  void Shutdown([[maybe_unused]] Crane::Scene::World &world) override {};
  void Update(Crane::Scene::World &world, f64 deltaTime) override;

private:
  void HandleKeyDown(Crane::Events::KeyDown &event);
  void HandleKeyUp(Crane::Events::KeyUp &event);

private:
  Crane::Scene::World *m_World = nullptr;
  Crane::Scene::Entity *m_PlayerEntity = nullptr;
  Crane::Core::Application *m_App = nullptr;
  PlayerComponent *m_PlayerComponent = nullptr;

  const f32 SPEED = 10.0;
};