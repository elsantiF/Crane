#pragma once

#include "Application/Application.hpp"
#include "Events/KeyDown.hpp"
#include "Events/KeyUp.hpp"
#include "PlayerComponent.hpp"
#include "Systems/ISystem.hpp"
#include "World/Entity.hpp"

class PlayerSystem : public Crane::Systems::IUpdateSystem {
public:
  PlayerSystem(Crane::Application::Application &app, Crane::World::Entity &playerEntity) : m_PlayerEntity(&playerEntity), m_App(&app) {}

  void Initialize(Crane::World::World &world) override;

  void Shutdown([[maybe_unused]] Crane::World::World &world) override {};
  void Update(Crane::World::World &world, f64 deltaTime) override;

private:
  void HandleKeyDown(Crane::Events::KeyDownEvent &event);
  void HandleKeyUp(Crane::Events::KeyUpEvent &event);

private:
  Crane::World::World *m_World = nullptr;
  Crane::World::Entity *m_PlayerEntity = nullptr;
  Crane::Application::Application *m_App = nullptr;
  PlayerComponent *m_PlayerComponent = nullptr;

  const f32 SPEED = 10.0;
};