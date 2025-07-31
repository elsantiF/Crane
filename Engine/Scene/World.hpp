#pragma once

#include "Base/Types.hpp"
#include "Core/Systems/SystemManager.hpp"
#include <entt/entity/registry.hpp>

namespace Crane::Scene {
  using Entity = entt::entity;

  class World {
  public:
    World() : m_SystemManager(*this) {};
    ~World() = default;

    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);

    // --- Entity management section ---
    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template <typename T, typename... Args>
    void AddComponent(Entity entity, Args &&...args) {
      m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    T &GetComponent(Entity entity) {
      return m_Registry.get<T>(entity);
    }

    template <typename T>
    const T &GetComponent(Entity entity) const {
      return m_Registry.get<T>(entity);
    }

    template <typename T>
    bool HasComponent(Entity entity) const {
      return m_Registry.all_of<T>(entity);
    }

    template <typename T>
    const T *TryGetComponent(Entity entity) const {
      return m_Registry.try_get<T>(entity);
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
      m_Registry.remove<T>(entity);
    }

    // --- Accessors ---
    entt::registry &GetRegistry();
    Core::Systems::SystemManager &GetSystemManager();

  private:
    entt::registry m_Registry;
    Core::Systems::SystemManager m_SystemManager;
  };
}
