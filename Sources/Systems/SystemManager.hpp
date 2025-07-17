#pragma once

#include "Core/Types.hpp"
#include "Systems/ISystem.hpp"

namespace Crane::Systems {
  class SystemManager {
  public:
    SystemManager(World::World &world) : m_World(world) {}
    ~SystemManager() = default;

    template <typename T, typename... Args>
    void AddSystem(Args &&...args) {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        m_FixedUpdateSystems.emplace_back(MakeScope<T>(std::forward<Args>(args)...));
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        m_UpdateSystems.emplace_back(MakeScope<T>(std::forward<Args>(args)...));
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
      return nullptr;
    }

    template <typename T>
    void RemoveSystem() {
      if constexpr (std::is_base_of_v<IFixedUpdateSystem, T>) {
        m_FixedUpdateSystems.erase(std::remove_if(m_FixedUpdateSystems.begin(), m_FixedUpdateSystems.end(),
                                                  [](const Scope<IFixedUpdateSystem> &system) {
                                                    return dynamic_cast<T *>(system.get()) != nullptr;
                                                  }),
                                   m_FixedUpdateSystems.end());
      } else if constexpr (std::is_base_of_v<IUpdateSystem, T>) {
        m_UpdateSystems.erase(std::remove_if(m_UpdateSystems.begin(), m_UpdateSystems.end(),
                                             [](const Scope<IUpdateSystem> &system) {
                                               return dynamic_cast<T *>(system.get()) != nullptr;
                                             }),
                              m_UpdateSystems.end());
      }
    }

    void UpdateSystems(f64 deltaTime);
    void FixedUpdateSystems(f64 deltaTime);

  private:
    Vector<Scope<IFixedUpdateSystem>> m_FixedUpdateSystems;
    Vector<Scope<IUpdateSystem>> m_UpdateSystems;
    World::World &m_World;
  };
}