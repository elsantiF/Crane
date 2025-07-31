#pragma once

#include "Base/Logger.hpp"
#include "Base/Types.hpp"
#include "ISystem.hpp"
#include <typeindex>

namespace Crane::Systems {
  class SystemManager {
  public:
    SystemManager(Scene::World &world) : m_World(world) {}
    ~SystemManager() = default;

    template <typename T, typename... Args>
    void AddSystem(Args &&...args) {
      auto system = MakeScope<T>(std::forward<Args>(args)...);
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        if (m_FixedUpdateSystems.find(typeid(T)) != m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} already exists", typeid(T).name());
          return;
        }

        m_FixedUpdateSystems[typeid(T)] = std::move(system);
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        if (m_UpdateSystems.find(typeid(T)) != m_UpdateSystems.end()) {
          Logger::Error("System of type {} already exists", typeid(T).name());
          return;
        }

        m_UpdateSystems[typeid(T)] = std::move(system);
      } else {
        Logger::Error("System type must derive from IFixedUpdateSystem or IUpdateSystem");
        return;
      }
    }

    template <typename T>
    T *GetSystem() {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        if (m_FixedUpdateSystems.find(typeid(T)) == m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return nullptr;
        }

        return dynamic_cast<T *>(m_FixedUpdateSystems[typeid(T)].get());
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        if (m_UpdateSystems.find(typeid(T)) == m_UpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return nullptr;
        }

        return dynamic_cast<T *>(m_UpdateSystems[typeid(T)].get());
      }
    }

    template <typename T>
    void RemoveSystem() {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        auto it = m_FixedUpdateSystems.find(typeid(T));
        if (it == m_FixedUpdateSystems.end()) {
          Logger::Error("System of type {} not found", typeid(T).name());
          return;
        }
        m_FixedUpdateSystems.erase(it);
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
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

    void UpdateSystems(f64 deltaTime);
    void FixedUpdateSystems(f64 deltaTime);

  private:
    UnorderedMap<std::type_index, Scope<IFixedUpdateSystem>> m_FixedUpdateSystems;
    UnorderedMap<std::type_index, Scope<IUpdateSystem>> m_UpdateSystems;
    Scene::World &m_World;
  };
}