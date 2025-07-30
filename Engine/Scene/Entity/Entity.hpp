#pragma once

#include <entt/entity/entity.hpp>

namespace Crane::Scene {
  class World;

  class Entity {
  public:
    Entity() = default;
    Entity(Scene::World *world, entt::entity handle) : m_Entity(handle), m_World(world) {}

    entt::entity GetHandle() const {
      return m_Entity;
    }

    template <typename T, typename... Args>
    void AddComponent(Args &&...args);

    template <typename T>
    T &GetComponent();

    template <typename T>
    const T &GetComponent() const;

    template <typename T>
    bool HasComponent() const;

  private:
    entt::entity m_Entity{entt::null};
    Scene::World *m_World = nullptr;
  };
}

#include "Entity.inl"