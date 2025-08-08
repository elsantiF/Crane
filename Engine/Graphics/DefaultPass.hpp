#pragma once

#include "Graphics/RenderPass.hpp"

namespace Crane::Graphics {
  class DefaultPass : public RenderPass {
  public:
    DefaultPass(IRenderer &renderer);

    void Render(Scene::World &world) override;

  private:
    Id m_PipelineId;
    Id m_SamplerId;
  };
}
