#include "World.hpp"
#include "Entity.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/RenderingSystem.hpp"

namespace Crane::World {
  World::World() : m_PhysicsWorld() {
    m_UpdateSystems.emplace_back(new Systems::PhysicsSystem());
    m_RenderSystems.emplace_back(new Systems::RenderingSystem());
  }

  void World::Update(f64 deltaTime) {
    for (auto system : m_UpdateSystems) {
      system->Update(*this, deltaTime);
    }
  }

  void World::Render(Graphics::IRenderer &renderer) {
    for (auto system : m_RenderSystems) {
      system->Render(*this, renderer);
    }
  }

  Entity World::CreateEntity() {
    auto entity = m_Registry.create();
    return Entity(this, entity);
  }
}