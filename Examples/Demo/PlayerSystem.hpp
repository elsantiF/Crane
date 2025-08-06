#pragma once

#include "Core/Systems/ISystem.hpp"
#include "Events/Events.hpp"
#include "PlayerComponent.hpp"
#include "Scene/World.hpp"

class PlayerSystem : public Crane::Systems::IFixedUpdateSystem {
public:
  PlayerSystem(Crane::Scene::World &world, Crane::Scene::Entity &playerEntity);
  ~PlayerSystem() override = default;

  void FixedUpdate(f64 deltaTime) override;

private:
  void HandleKeyDown(Crane::Events::KeyDown &event);
  void HandleKeyUp(Crane::Events::KeyUp &event);

private:
  Crane::Scene::Entity *m_PlayerEntity = nullptr;
  PlayerComponent *m_PlayerComponent = nullptr;

  const f32 SPEED = 10.0;
};