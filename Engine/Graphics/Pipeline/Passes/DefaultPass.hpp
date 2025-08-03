#include "Graphics/Pipeline/RenderPass.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/World.hpp"

namespace Crane::Graphics {
  class DefaultPass : public RenderPass {
  public:
    DefaultPass(IRenderer &renderer) : RenderPass(renderer) {}
    ~DefaultPass() override = default;

    void Begin() override {}

    void Execute(Scene::World &world) override {
      auto &registry = world.GetRegistry();
      auto view = registry.view<Scene::Components::Transform, Scene::Components::Renderable>();

      for (auto entity : view) {
        const auto &[transform, renderable] = view.get<Scene::Components::Transform, Scene::Components::Renderable>(entity);

        m_Renderer.SetTexture(renderable.textureId);
        m_Renderer.SetMesh(renderable.meshId);
        m_Renderer.DrawRenderable(transform.transform);
      }
    }

    void End() override {}
  };
}