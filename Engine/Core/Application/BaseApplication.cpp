#include "BaseApplication.hpp"
#include "Base/Clock.hpp"
#include "Base/Profiler.hpp"

namespace Crane {
  void BaseApplication::Initialize() {
    PROFILE_SCOPE();
    Logger::Initialize();

    PlatformInitialize();

    m_World = MakeScope<Scene::World>();
    m_Running = true;
    OnInitialize();
  }

  void BaseApplication::Run() {
    f64 lastTime = Clock::GetCurrentTime();
    f64 accumulator = 0.0;

    while (m_Running) {
      f64 currentTime = Clock::GetCurrentTime();
      f64 deltaTime = currentTime - lastTime;
      accumulator += deltaTime;

      HandleEvents();

      while (accumulator >= PHYSICS_TIMESTEP) {
        FixedUpdate(PHYSICS_TIMESTEP);
        accumulator -= PHYSICS_TIMESTEP;
      }

      Update(deltaTime);

      OnPreRender();
      Render();
      OnPostRender();

      lastTime = currentTime;
    }
  }

  void BaseApplication::FixedUpdate(f64 deltaTime) {
    PROFILE_SCOPE();
    OnPreFixedUpdate();
    m_World->FixedUpdate(deltaTime);
    OnPostFixedUpdate();
  }

  void BaseApplication::Update(f64 deltaTime) {
    PROFILE_SCOPE();
    OnPreUpdate();
    m_World->Update(deltaTime);
    OnPostUpdate();
  }
}