#include "Application/ClientApplication.hpp"
#include "CelestialBody.hpp"
#include "Graphics/TextureManager.hpp"
#include "GravitySystem.hpp"
#include "Scene/Components/Renderable.hpp"
#include "Scene/Components/Transform.hpp"
#include <numbers>

using namespace Crane;

Graphics::Mesh CreateCircleVertices(f32 radius, i32 segments, Graphics::Color color) {
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

  Vector<i32> indices;
  for (i32 i = 0; i < segments; ++i) {
    indices.push_back(i);
    indices.push_back((i + 1) % segments);
    indices.push_back(segments);
  }

  return Graphics::Mesh{vertices, indices};
}

static Graphics::TextureManager textureManager;

const ApplicationInfo appInfo = {
    "Crane NBody", {1,    0,   0   },
     {1600, 900, true}
};

enum class PlanetColor { Red, Green, Purple };

class NBody : public ClientApplication {
public:
  NBody() : ClientApplication(appInfo) {}

  void OnInitialize() override {
    m_PlanetMeshId = m_Renderer->LoadMesh(CreateCircleVertices(24.0f, 24, Graphics::Colors::White));

    auto redTexture = textureManager.LoadTexture("Resources/red.png").value();
    auto greenTexture = textureManager.LoadTexture("Resources/green.png").value();
    auto purpleTexture = textureManager.LoadTexture("Resources/purple.png").value();
    m_RedTextureId = m_Renderer->LoadTexture(redTexture);
    m_GreenTextureId = m_Renderer->LoadTexture(greenTexture);
    m_PurpleTextureId = m_Renderer->LoadTexture(purpleTexture);

    // Temporary: Create a few planets
    CreatePlanet(GetWorld(), Math::Vec2f{100.0f, 300.0f}, 500.0f, 24.0f, Math::Vec2f{5.0f, 0.0f}, PlanetColor::Red);
    CreatePlanet(GetWorld(), Math::Vec2f{600.0f, 400.0f}, 500.0f, 24.0f, Math::Vec2f{-1.0f, -2.0f}, PlanetColor::Green);
    CreatePlanet(GetWorld(), Math::Vec2f{900.0f, 800.0f}, 500.0f, 24.0f, Math::Vec2f{0.0f, 10.0f}, PlanetColor::Purple);

    GetWorld().AddSystem<GravitySystem>();
  }

  void OnImGui() override {}

  Scene::Entity CreatePlanet(Scene::World &world, const Math::Vec2f &position, f64 mass, f64 radius, Math::Vec2f velocity, PlanetColor color) {
    Scene::Entity planet = world.CreateEntity();
    world.AddComponent<Scene::Components::Transform>(planet, position);
    world.AddComponent<CelestialBody>(planet, CelestialBody{mass, radius, velocity});

    switch (color) {
    case PlanetColor::Red:    world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMeshId, m_RedTextureId); break;
    case PlanetColor::Green:  world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMeshId, m_GreenTextureId); break;
    case PlanetColor::Purple: world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMeshId, m_PurpleTextureId); break;
    }

    return planet;
  }

private:
  Id m_PlanetMeshId;
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