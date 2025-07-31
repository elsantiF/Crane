#pragma once

#include "Base/Types.hpp"

namespace Crane::Resources {
  template <typename T>
  struct Resource {
  public:
    Resource() = default;
    Resource(Ref<T> data) : m_Data(std::move(data)) {}

    Resource(const Resource &) = default;
    Resource(Resource &&) = default;

    Resource &operator=(const Resource &) = default;
    Resource &operator=(Resource &&) = default;

    T &operator*() {
      return *m_Data;
    }
    T *operator->() {
      return m_Data.get();
    }
    T *get() {
      return m_Data.get();
    }

    explicit operator bool() const {
      return static_cast<bool>(m_Data);
    }
    bool Valid() const {
      return static_cast<bool>(m_Data);
    }

    u64 UseCount() const {
      return m_Data ? m_Data->UseCount() : 0;
    }

  public:
    Ref<T> m_Data;
  };
}