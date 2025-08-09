#pragma once

#include "Graphics/Renderer/IRenderer.hpp"
#include "Scene/World.hpp"

namespace Crane::Graphics {
  enum class LoadOperation { Clear, Load, DontCare };
  enum class StoreOperation { Store, DontCare };

  struct RenderPassConfig {
    Color clearColor = Colors::CLEAR_COLOR;
    LoadOperation loadOp = LoadOperation::Clear;
    StoreOperation storeOp = StoreOperation::Store;
  };

  class RenderPass {
  public:
    RenderPass(IRenderer &renderer, const RenderPassConfig &config) : m_Renderer(renderer), m_Config(config) {}
    virtual ~RenderPass() = default;

    RenderPassConfig GetConfig() const {
      return m_Config;
    }

    virtual void Render(Scene::World &world) = 0;

  protected:
    IRenderer &m_Renderer;
    RenderPassConfig m_Config;
  };
}