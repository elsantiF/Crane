#include "PlayerSystem.hpp"
#include "Components/Transform.hpp"
#include "PlayerComponent.hpp"

void PlayerSystem::Initialize(Crane::World::World &world) {
  m_World = &world;
  m_App->GetDispatcher().sink<Crane::Events::KeyDownEvent>().connect<&PlayerSystem::HandleKeyDown>(this);
  m_App->GetDispatcher().sink<Crane::Events::KeyUpEvent>().connect<&PlayerSystem::HandleKeyUp>(this);

  m_PlayerComponent = &m_PlayerEntity->GetComponent<PlayerComponent>();
}

void PlayerSystem::Update([[maybe_unused]] Crane::World::World &world, f64 deltaTime) {
  auto &transform = m_PlayerEntity->GetComponent<Crane::Components::Transform>();
  if (m_PlayerComponent->isMovingLeft) {
    transform.transform.position.x -= SPEED * deltaTime;
    transform.dirty = true;
  }
  if (m_PlayerComponent->isMovingRight) {
    transform.transform.position.x += SPEED * deltaTime;
    transform.dirty = true;
  }
}

void PlayerSystem::HandleKeyDown(Crane::Events::KeyDownEvent &event) {
  if (event.GetKeyCode() == SDLK_A) {
    m_PlayerComponent->isMovingLeft = true;
  } else if (event.GetKeyCode() == SDLK_D) {
    m_PlayerComponent->isMovingRight = true;
  }
}

void PlayerSystem::HandleKeyUp(Crane::Events::KeyUpEvent &event) {
  if (event.GetKeyCode() == SDLK_A) {
    m_PlayerComponent->isMovingLeft = false;
  } else if (event.GetKeyCode() == SDLK_D) {
    m_PlayerComponent->isMovingRight = false;
  }
}