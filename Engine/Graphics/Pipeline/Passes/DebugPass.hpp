#include "Base/Profiler.hpp"
#include "Graphics/IRenderer.hpp"
#include "Graphics/Pipeline/RenderPass.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/CircleCollider.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include "Scene/World.hpp"

namespace Crane::Graphics {
  class DebugPass : public RenderPass {
  public:
    DebugPass(IRenderer &renderer, f32 pixelsPerMeter) : RenderPass(renderer), m_PPM(pixelsPerMeter) {}
    ~DebugPass() override = default;

    void Begin() override {}

    void Execute(Scene::World &world) override {
      PROFILE_SCOPE();
      constexpr f32 PI = 3.14159265359f;

      auto &registry = world.GetRegistry();
      auto boxView = registry.view<Scene::Components::Transform, Scene::Components::Renderable, Scene::Components::BoxCollider>();

      for (auto entity : boxView) {
        const auto &[transform, renderable, boxCollider] =
            boxView.get<Scene::Components::Transform, Scene::Components::Renderable, Scene::Components::BoxCollider>(entity);

        Math::Vec2f halfDimensions = boxCollider.dimensions * 0.5f * m_PPM;
        Math::Vec2f topLeft = transform.transform.position + Math::Vec2f{-halfDimensions.x, -halfDimensions.y};
        Math::Vec2f topRight = transform.transform.position + Math::Vec2f{halfDimensions.x, -halfDimensions.y};
        Math::Vec2f bottomLeft = transform.transform.position + Math::Vec2f{-halfDimensions.x, halfDimensions.y};
        Math::Vec2f bottomRight = transform.transform.position + Math::Vec2f{halfDimensions.x, halfDimensions.y};

        float angle = transform.transform.rotation;
        Math::Vec2f center = transform.transform.position;
        auto rotate = [&](Math::Vec2f point) {
          float cosAngle = std::cos(angle);
          float sinAngle = std::sin(angle);
          return Math::Vec2f{center.x + (point.x - center.x) * cosAngle - (point.y - center.y) * sinAngle,
                             center.y + (point.x - center.x) * sinAngle + (point.y - center.y) * cosAngle};
        };

        topLeft = rotate(topLeft);
        topRight = rotate(topRight);
        bottomLeft = rotate(bottomLeft);
        bottomRight = rotate(bottomRight);

        m_Renderer.SetFillColor(Colors::White);
        m_Renderer.DrawLine(topLeft, topRight);
        m_Renderer.DrawLine(topRight, bottomRight);
        m_Renderer.DrawLine(bottomRight, bottomLeft);
        m_Renderer.DrawLine(bottomLeft, topLeft);
        m_Renderer.SetFillColor(Colors::Yellow);
        m_Renderer.DrawPoint(transform.transform.position);
      }

      auto circleView = registry.view<Scene::Components::Transform, Scene::Components::Renderable, Scene::Components::CircleCollider>();
      for (auto entity : circleView) {
        const auto &[transform, renderable, circleCollider] =
            circleView.get<Scene::Components::Transform, Scene::Components::Renderable, Scene::Components::CircleCollider>(entity);

        Math::Vec2f center = transform.transform.position;
        float radius = circleCollider.radius * 30.0f;
        int segments = 16;
        float angleStep = 2.0f * PI / segments;

        m_Renderer.SetFillColor(Colors::White);
        for (int i = 0; i < segments; ++i) {
          float angle1 = i * angleStep;
          float angle2 = (i + 1) * angleStep;
          Math::Vec2f point1 = center + Math::Vec2f{radius * std::cos(angle1), radius * std::sin(angle1)};
          Math::Vec2f point2 = center + Math::Vec2f{radius * std::cos(angle2), radius * std::sin(angle2)};
          m_Renderer.DrawLine(point1, point2);
        }
        m_Renderer.SetFillColor(Colors::Yellow);
        m_Renderer.DrawPoint(transform.transform.position);
      }
    }

    void End() override {}

  private:
    f32 m_PPM;
  };
}