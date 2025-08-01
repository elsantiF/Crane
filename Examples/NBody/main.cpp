#include "Application/ClientApplication.hpp"
#include "Graphics/TextureManager.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include <numbers>

using namespace Crane;

Graphics::SVertex2List CreateCircleVertices(f32 radius, i32 segments, Graphics::Color color) {
  Graphics::SVertex2List vertices;
  for (i32 i = 0; i < segments; ++i) {
    f32 angle = (2.0f * std::numbers::pi_v<f32> * i) / segments;
    f32 u = (cos(angle) + 1.0f) * 0.5f;
    f32 v = (sin(angle) + 1.0f) * 0.5f;
    vertices.push_back({
        {radius * cos(angle), radius * sin(angle)},
        color, {u,                   v                  }
    });
  }

  vertices.push_back({
      {0.0f, 0.0f},
      color, {0.5f, 0.5f}
  });
  return vertices;
}

Vector<i32> CreateCircleIndices(i32 segments) {
  Vector<i32> indices;
  for (i32 i = 0; i < segments; ++i) {
    indices.push_back(i);
    indices.push_back((i + 1) % segments);
    indices.push_back(segments);
  }
  return indices;
}

static Graphics::TextureManager textureManager;

const ApplicationInfo appInfo = {
    "Crane NBody", {1,    0,   0   },
     {1600, 900, true}
};

class NBody : public ClientApplication {
public:
  NBody() : ClientApplication(appInfo) {}

  void OnInitialize() override {
    m_PlanetVerticesId = m_Renderer->LoadVertexData(CreateCircleVertices(48.0f, 24, Graphics::Colors::White));
    m_PlanetIndicesId = m_Renderer->LoadIndexData(CreateCircleIndices(24));

    auto redTexture = textureManager.LoadTexture("Resources/red.png");
    auto greenTexture = textureManager.LoadTexture("Resources/green.png");
    auto purpleTexture = textureManager.LoadTexture("Resources/purple.png");
    m_RedTextureId = m_Renderer->LoadTexture(*redTexture);
    m_GreenTextureId = m_Renderer->LoadTexture(*greenTexture);
    m_PurpleTextureId = m_Renderer->LoadTexture(*purpleTexture);

    // Temporary: Create a few planets
    Scene::Entity redPlanet = GetWorld().CreateEntity();
    GetWorld().AddComponent<Scene::Components::Transform>(redPlanet, Math::Vec2f{300.0f, 300.0f});
    GetWorld().AddComponent<Scene::Components::Renderable>(redPlanet, m_PlanetVerticesId, m_PlanetIndicesId, m_RedTextureId);

    Scene::Entity greenPlanet = GetWorld().CreateEntity();
    GetWorld().AddComponent<Scene::Components::Transform>(greenPlanet, Math::Vec2f{600.0f, 300.0f});
    GetWorld().AddComponent<Scene::Components::Renderable>(greenPlanet, m_PlanetVerticesId, m_PlanetIndicesId, m_GreenTextureId);

    Scene::Entity purplePlanet = GetWorld().CreateEntity();
    GetWorld().AddComponent<Scene::Components::Transform>(purplePlanet, Math::Vec2f{900.0f, 300.0f});
    GetWorld().AddComponent<Scene::Components::Renderable>(purplePlanet, m_PlanetVerticesId, m_PlanetIndicesId, m_PurpleTextureId);
  }

  void OnPreFixedUpdate() override {}
  void OnPostFixedUpdate() override {}
  void OnPreUpdate() override {}
  void OnPostUpdate() override {}
  void OnPreRender() override {}
  void OnPostRender() override {}
  void OnImGui() override {}

private:
  Id m_PlanetVerticesId;
  Id m_PlanetIndicesId;
  Id m_RedTextureId;
  Id m_GreenTextureId;
  Id m_PurpleTextureId;
};

int main() {
  NBody nbody;
  nbody.Initialize();
  nbody.Run();
  return 0;
}