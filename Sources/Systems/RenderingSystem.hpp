#pragma once

namespace Crane {
  namespace Graphics {
    class IRenderer;
  }
  namespace World {
    class World;
  }
}

namespace Crane::Systems {
  class RenderingSystem {
  public:
    RenderingSystem(Graphics::IRenderer &renderer, World::World &world) : m_Renderer(renderer), m_World(world) {}

    void Render();

  private:
    Graphics::IRenderer &m_Renderer;
    World::World &m_World;
  };
}