#pragma once

#include "Graphics/IRenderer.hpp"
#include "Graphics/Pipeline/RenderPass.hpp"
#include "Scene/World.hpp"
#include "Trail.hpp"

class TrailPass : public Crane::Graphics::RenderPass {
public:
  TrailPass(Crane::Graphics::IRenderer &renderer) : RenderPass(renderer) {}
  ~TrailPass() override = default;

  void Begin() override {}

  void Execute(Crane::Scene::World &world) override {
    auto &registry = world.GetRegistry();
    auto view = registry.view<Trail>();
    for (auto entity : view) {
      const auto &trail = view.get<Trail>(entity);
      if (trail.points.size() < 2) {
        continue;
      }

      auto lastPoint = trail.points[0];
      for (size_t i = 1; i < trail.points.size(); ++i) {
        const auto &point = trail.points[i];
        m_Renderer.SetFillColor({1.0f, 0.0f, 0.0f, 1.0f});
        m_Renderer.DrawLine(lastPoint, point);
        lastPoint = point;
      }
    }
  }

  void End() override {}
};