#pragma once

#include "Base/Profiler.hpp"
#include "World.hpp"

namespace Crane::Scene {
  template <typename T, typename... Args>
  void World::AddComponent(Entity entity, Args &&...args) {
    PROFILE_SCOPE();
    m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T &World::GetComponent(Entity entity) {
    PROFILE_SCOPE();
    return m_Registry.get<T>(entity);
  }

  template <typename T>
  const T &World::GetComponent(Entity entity) const {
    PROFILE_SCOPE();
    return m_Registry.get<T>(entity);
  }

  template <typename T>
  bool World::HasComponent(Entity entity) const {
    PROFILE_SCOPE();
    return m_Registry.all_of<T>(entity);
  }

  template <typename T>
  const T *World::TryGetComponent(Entity entity) const {
    PROFILE_SCOPE();
    return m_Registry.try_get<T>(entity);
  }

  template <typename T>
  void World::RemoveComponent(Entity entity) {
    PROFILE_SCOPE();
    m_Registry.remove<T>(entity);
  }
}