#pragma once

#include "Base/Logger.hpp"
#include "Base/Types.hpp"
#include "Systems/ISystem.hpp"

namespace Crane::Systems {
  class SystemManager {
  public:
    SystemManager(Scene::World &world) : m_World(world) {}
    ~SystemManager() = default;

    template <typename T, typename... Args>
    void AddSystem(Args &&...args) {
      auto system = MakeScope<T>(std::forward<Args>(args)...);
      system->Initialize(m_World);

      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        m_FixedUpdateSystems.emplace_back(std::move(system));
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        m_UpdateSystems.emplace_back(std::move(system));
      } else {
        Logger::Error("System type must derive from IFixedUpdateSystem or IUpdateSystem");
        return;
      }
    }

    template <typename T>
    T *GetSystem() {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        for (auto &system : m_FixedUpdateSystems) {
          if (auto ptr = dynamic_cast<T *>(system.get())) {
            return ptr;
          }
        }
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        for (auto &system : m_UpdateSystems) {
          if (auto ptr = dynamic_cast<T *>(system.get())) {
            return ptr;
          }
        }
      }

      Logger::Error("System not found");
      return nullptr;
    }

    template <typename T>
    void RemoveSystem() {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        auto it = m_FixedUpdateSystems.begin();
        while (it != m_FixedUpdateSystems.end()) {
          if (auto ptr = dynamic_cast<T *>(it->get())) {
            ptr->Shutdown(m_World);
            it = m_FixedUpdateSystems.erase(it);
          } else {
            ++it;
          }
        }
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        auto it = m_UpdateSystems.begin();
        while (it != m_UpdateSystems.end()) {
          if (auto ptr = dynamic_cast<T *>(it->get())) {
            ptr->Shutdown(m_World);
            it = m_UpdateSystems.erase(it);
          } else {
            ++it;
          }
        }
      } else {
        Logger::Error("System type must derive from IFixedUpdateSystem or IUpdateSystem");
      }
    }

    void UpdateSystems(f64 deltaTime);
    void FixedUpdateSystems(f64 deltaTime);

  private:
    Vector<Scope<IFixedUpdateSystem>> m_FixedUpdateSystems;
    Vector<Scope<IUpdateSystem>> m_UpdateSystems;
    Scene::World &m_World;
  };
}