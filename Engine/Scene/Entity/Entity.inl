#include "Scene/World/World.hpp"

namespace Crane::Scene {
  template <typename T, typename... Args>
  void Entity::AddComponent(Args &&...args) {
    m_World->GetRegistry().emplace<T>(m_Entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T &Entity::GetComponent() {
    return m_World->GetRegistry().get<T>(m_Entity);
  }

  template <typename T>
  const T &Entity::GetComponent() const {
    return m_World->GetRegistry().get<T>(m_Entity);
  }

  template <typename T>
  bool Entity::HasComponent() const {
    return m_World->GetRegistry().all_of<T>(m_Entity);
  }
}