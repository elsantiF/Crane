#pragma once

#include "Base/Logger.hpp"
#include "Base/Types.hpp"
#include "Core/Systems/ISystem.hpp"
#include <entt/entity/registry.hpp>
#include <typeindex>

namespace Crane::Scene {
  using Entity = entt::entity;

  class World {
  public:
    World() = default;
    ~World() = default;
    World(const World &) = delete;
    World &operator=(const World &) = delete;

    void FixedUpdate(f64 deltaTime);
    void Update(f64 deltaTime);

    // --- System management section ---
    template <typename T, typename... Args>
    T *AddSystem(Args &&...args) {
      auto system = MakeScope<T>(std::forward<Args>(args)...);
      T *systemPtr = system.get();

      if constexpr (std::is_base_of_v<Systems::IFixedUpdateSystem, T>) {
        if (m_FixedUpdateSystems.find(typeid(T)) != m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} already exists", typeid(T).name());
          return nullptr;
        }

        m_FixedUpdateSystems[typeid(T)] = std::move(system);
        return systemPtr;
      } else if constexpr (std::is_base_of_v<Systems::IUpdateSystem, T>) {
        if (m_UpdateSystems.find(typeid(T)) != m_UpdateSystems.end()) {
          Logger::Error("System of type {} already exists", typeid(T).name());
          return nullptr;
        }

        m_UpdateSystems[typeid(T)] = std::move(system);
        return systemPtr;
      } else {
        Logger::Error("System type must derive from IFixedUpdateSystem or IUpdateSystem");
        return nullptr;
      }
    }

    template <typename T>
    T *GetSystem() {
      if constexpr (std::is_base_of_v<Systems::IFixedUpdateSystem, T>) {
        if (m_FixedUpdateSystems.find(typeid(T)) == m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return nullptr;
        }

        return dynamic_cast<T *>(m_FixedUpdateSystems[typeid(T)].get());
      } else if constexpr (std::is_base_of_v<Systems::IUpdateSystem, T>) {
        if (m_UpdateSystems.find(typeid(T)) == m_UpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return nullptr;
        }

        return dynamic_cast<T *>(m_UpdateSystems[typeid(T)].get());
      }
    }

    template <typename T>
    void RemoveSystem() {
      if constexpr (std::is_base_of_v<Systems::IFixedUpdateSystem, T>) {
        auto it = m_FixedUpdateSystems.find(typeid(T));
        if (it == m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return;
        }
        m_FixedUpdateSystems.erase(it);
      } else if constexpr (std::is_base_of_v<Systems::IUpdateSystem, T>) {
        auto it = m_UpdateSystems.find(typeid(T));
        if (it == m_UpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return;
        }
        m_UpdateSystems.erase(it);
      } else {
        Logger::Error("System type must derive from IFixedUpdateSystem or IUpdateSystem");
      }
    }

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

  private:
    entt::registry m_Registry;
    UnorderedMap<std::type_index, Scope<Systems::IFixedUpdateSystem>> m_FixedUpdateSystems;
    UnorderedMap<std::type_index, Scope<Systems::IUpdateSystem>> m_UpdateSystems;
  };
}
