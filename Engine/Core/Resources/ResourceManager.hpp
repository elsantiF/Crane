#pragma once

#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include "Base/Types.hpp"
#include "Resource.hpp"
#include <functional>

namespace Crane::Resources {
  enum class ResourceManagerError { LoaderNotSet, ResourceNotFound, LoadFailed, InvalidResource, UndefinedError };

  template <typename T>
  class ResourceManager {
  public:
    using LoaderFunc = Function<Expected<Resource<T>, ResourceManagerError>(const Path &)>;

    ResourceManager() = default;
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    void SetLoader(LoaderFunc loader) {
      m_Loader = std::move(loader);
    }

    Expected<Resource<T>, ResourceManagerError> Load(const Path &path) {
      PROFILE_SCOPE();
      auto hash = std::hash<Path>{}(path);

      auto it = m_Cache.find(hash);
      if (it != m_Cache.end()) {
        return it->second;
      }

      if (!m_Loader) {
        Logger::Error("ResourceManager: Loader function is not set.");
        return std::unexpected<ResourceManagerError>(ResourceManagerError::LoaderNotSet);
      }

      auto data = m_Loader(path);
      if (!data) {
        Logger::Error("ResourceManager: Failed to load resource.");
        return std::unexpected<ResourceManagerError>(ResourceManagerError::LoadFailed);
      }

      Resource<T> resource{data.value()};
      m_Cache.emplace(hash, resource);
      return resource;
    }

    Resource<T> Reload(const Path &path) {
      auto hash = std::hash<Path>{}(path);
      auto it = m_Cache.erase(hash);
      return Load(path);
    }

    Expected<Resource<T>, ResourceManagerError> GetResource(const Path &path) {
      PROFILE_SCOPE();
      auto hash = std::hash<Path>{}(path);
      auto it = m_Cache.find(hash);
      if (it != m_Cache.end()) {
        return *it->second;
      }

      Logger::Error("ResourceManager: Resource not found for path: {}", path.generic_string());
      return std::unexpected<ResourceManagerError>(ResourceManagerError::ResourceNotFound);
    }

    void Clear() {
      m_Cache.clear();
    }

    u64 Size() const {
      return m_Cache.size();
    }

    bool Contains(const Path &path) const {
      auto hash = std::hash<Path>{}(path);
      return m_Cache.find(hash) != m_Cache.end();
    }

    void Remove(const Path &path) {
      auto hash = std::hash<Path>{}(path);
      m_Cache.erase(hash);
    }

    void GarbageCollect() {
      PROFILE_SCOPE();
      for (auto it = m_Cache.begin(); it != m_Cache.end();) {
        if (!it->second.Valid()) {
          it = m_Cache.erase(it);
        } else {
          ++it;
        }
      }
    }

  protected:
    UnorderedMap<size_t, Resource<T>> m_Cache;
    LoaderFunc m_Loader;
  };
}