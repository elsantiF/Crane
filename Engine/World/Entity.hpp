#pragma once

#include <entt/entt.hpp>

namespace Crane::World {
  class World;

  class Entity {
  public:
    Entity() = default;
    Entity(World *world, entt::entity handle) : m_Entity(handle), m_World(world) {}

    entt::entity GetHandle() const {
      return m_Entity;
    }

    template <typename T, typename... Args>
    void AddComponent(Args &&...args);

    template <typename T>
    T &GetComponent();

  private:
    entt::entity m_Entity{entt::null};
    World *m_World = nullptr;
  };
}

#include "Entity.inl"