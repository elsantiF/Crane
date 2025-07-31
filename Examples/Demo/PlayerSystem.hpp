#pragma once

#include "Application/ClientApplication.hpp"
#include "Core/Systems/ISystem.hpp"
#include "Events/Events.hpp"
#include "PlayerComponent.hpp"

class PlayerSystem : public Crane::Systems::IUpdateSystem {
public:
  PlayerSystem(Crane::Scene::World &world, Crane::ClientApplication &app, Crane::Scene::Entity &playerEntity);

  void Update(Crane::Scene::World &world, f64 deltaTime) override;

private:
  void HandleKeyDown(Crane::Events::KeyDown &event);
  void HandleKeyUp(Crane::Events::KeyUp &event);

private:
  Crane::ClientApplication *m_App = nullptr;
  Crane::Scene::Entity *m_PlayerEntity = nullptr;
  PlayerComponent *m_PlayerComponent = nullptr;

  const f32 SPEED = 10.0;
};