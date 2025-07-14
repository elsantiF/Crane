#ifdef TRACY_ENABLE
#include "Profiler.hpp"
void *operator new(size_t size) {
  void *ptr = malloc(size);
  PROFILE_MEM_ALLOC(ptr, size);
  return ptr;
}

void operator delete(void *ptr) noexcept {
  PROFILE_MEM_FREE(ptr);
  free(ptr);
}

void *operator new[](size_t size) {
  void *ptr = malloc(size);
  PROFILE_MEM_ALLOC(ptr, size);
  return ptr;
}

void operator delete[](void *ptr) noexcept {
  PROFILE_MEM_FREE(ptr);
  free(ptr);
}
#endif