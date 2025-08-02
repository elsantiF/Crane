#include "PlayerSystem.hpp"
#include "Events/Events.hpp"
#include "PlayerComponent.hpp"
#include "Scene/Components/RigidBody.hpp"
#include "Scene/Components/Transform.hpp"
#include <SDL3/SDL_keycode.h>

using namespace Crane;

PlayerSystem::PlayerSystem(Scene::World &world, Scene::Entity &playerEntity) : Systems::IUpdateSystem(world), m_PlayerEntity(&playerEntity) {
  m_World.GetDispatcher().sink<Events::KeyDown>().connect<&PlayerSystem::HandleKeyDown>(this);
  m_World.GetDispatcher().sink<Events::KeyUp>().connect<&PlayerSystem::HandleKeyUp>(this);

  m_PlayerComponent = &m_World.GetComponent<PlayerComponent>(*m_PlayerEntity);
}

void PlayerSystem::Update([[maybe_unused]] f64 deltaTime) {
  auto &rb = m_World.GetComponent<Scene::Components::RigidBody>(*m_PlayerEntity);
  auto &transform = m_World.GetComponent<Scene::Components::Transform>(*m_PlayerEntity);

  Math::Vec2f position = transform.transform.position;
  if (m_PlayerComponent->isMovingLeft) {
    m_World.GetDispatcher().trigger(Events::ForceApplied{
        rb.bodyId, Math::Vec2f{-SPEED * 30.0f, 0},
         position
    });
  }
  if (m_PlayerComponent->isMovingRight) {
    m_World.GetDispatcher().trigger(Events::ForceApplied{
        rb.bodyId, Math::Vec2f{SPEED * 30.0f, 0},
         position
    });
  }
  if (m_PlayerComponent->isJumping) {
    m_World.GetDispatcher().trigger(Events::ForceApplied{
        rb.bodyId, Math::Vec2f{0, -SPEED * 2000.0f},
         position
    });
    m_PlayerComponent->isJumping = false;
  }
}

void PlayerSystem::HandleKeyDown(Crane::Events::KeyDown &event) {
  if (event.GetKeyCode() == SDLK_A) {
    m_PlayerComponent->isMovingLeft = true;
  } else if (event.GetKeyCode() == SDLK_D) {
    m_PlayerComponent->isMovingRight = true;
  } else if (event.GetKeyCode() == SDLK_SPACE) {
    m_PlayerComponent->isJumping = true;
  }
}

void PlayerSystem::HandleKeyUp(Crane::Events::KeyUp &event) {
  if (event.GetKeyCode() == SDLK_A) {
    m_PlayerComponent->isMovingLeft = false;
  } else if (event.GetKeyCode() == SDLK_D) {
    m_PlayerComponent->isMovingRight = false;
  } else if (event.GetKeyCode() == SDLK_SPACE) {
    m_PlayerComponent->isJumping = false;
  }
}