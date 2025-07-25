#include "World.hpp"

namespace Crane::World {
  template <typename T, typename... Args>
  void Entity::AddComponent(Args &&...args) {
    m_World->GetRegistry().emplace<T>(m_Entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T &Entity::GetComponent() {
    return m_World->GetRegistry().get<T>(m_Entity);
  }
}