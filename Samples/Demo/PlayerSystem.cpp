#include "PlayerSystem.hpp"
#include "Components/Transform.hpp"

void PlayerSystem::Initialize(Crane::World::World &world) {
  m_World = &world;
  m_App->GetDispatcher().sink<Crane::Events::KeyDownEvent>().connect<&PlayerSystem::HandleKeyPress>(this);
}

void PlayerSystem::HandleKeyPress(Crane::Events::KeyDownEvent &event) {
  if (event.GetKeyCode() == SDLK_A) {
    auto &transform = m_PlayerEntity->GetComponent<Crane::Components::Transform>();
    transform.transform.position += Crane::Math::Vec2f{-1.0f, 0.0f};
    transform.dirty = true;
  } else if (event.GetKeyCode() == SDLK_D) {
    auto &transform = m_PlayerEntity->GetComponent<Crane::Components::Transform>();
    transform.transform.position += Crane::Math::Vec2f{1.0f, 0.0f};
    transform.dirty = true;
  }
}