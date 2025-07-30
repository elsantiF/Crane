#include "PlayerSystem.hpp"
#include "PlayerComponent.hpp"
#include "Scene/Components/RigidBody.hpp"
#include <box2d/box2d.h>
#include <SDL3/SDL_keycode.h>

void PlayerSystem::Initialize(Crane::Scene::World &world) {
  m_World = &world;
  m_App->GetDispatcher().sink<Crane::Events::KeyDown>().connect<&PlayerSystem::HandleKeyDown>(this);
  m_App->GetDispatcher().sink<Crane::Events::KeyUp>().connect<&PlayerSystem::HandleKeyUp>(this);

  m_PlayerComponent = &m_PlayerEntity->GetComponent<PlayerComponent>();
}

void PlayerSystem::Update([[maybe_unused]] Crane::Scene::World &world, [[maybe_unused]] f64 deltaTime) {
  auto &rb = m_PlayerEntity->GetComponent<Crane::Scene::Components::RigidBody>();
  b2Vec2 position = b2Body_GetPosition(rb.bodyId);
  if (m_PlayerComponent->isMovingLeft) {
    b2Body_ApplyForce(rb.bodyId, b2Vec2(-SPEED, 0), position, true);
  }
  if (m_PlayerComponent->isMovingRight) {
    b2Body_ApplyForce(rb.bodyId, b2Vec2(SPEED, 0), position, true);
  }
  if (m_PlayerComponent->isJumping) {
    b2Body_ApplyForce(rb.bodyId, b2Vec2(0, -SPEED * 75), position, true);
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