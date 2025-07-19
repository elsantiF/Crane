#pragma once

#include "Application/Application.hpp"
#include "Events/KeyDown.hpp"
#include "Systems/ISystem.hpp"
#include "World/Entity.hpp"

class PlayerSystem : public Crane::Systems::IUpdateSystem {
public:
  PlayerSystem(Crane::Core::Application &app, Crane::World::Entity &playerEntity) : m_PlayerEntity(&playerEntity), m_App(&app) {}

  void Initialize(Crane::World::World &world) override;

  void Shutdown([[maybe_unused]] Crane::World::World &world) override {};
  void Update([[maybe_unused]] Crane::World::World &world, [[maybe_unused]] f64 deltaTime) override {};

private:
  void HandleKeyPress(Crane::Events::KeyDownEvent &event);

private:
  Crane::World::World *m_World = nullptr;
  Crane::World::Entity *m_PlayerEntity = nullptr;
  Crane::Core::Application *m_App = nullptr;
};