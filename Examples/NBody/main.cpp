#include "Application/ClientApplication.hpp"
#include "CelestialBody.hpp"
#include "Graphics/Components/Renderable.hpp"
#include "Graphics/Primitives/Mesh.hpp"
#include "Graphics/TextureManager.hpp"
#include "GravitySystem.hpp"
#include "Scene/Components/Transform.hpp"
#include "Trail.hpp"
// #include "TrailPass.hpp"

using namespace Crane;

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
    // auto &renderer = m_RenderPipeline->GetRenderer();
    // m_RenderPipeline->AddRenderPass(MakeScope<TrailPass>(renderer));

    m_PlanetMesh = m_RenderSystem->CreateMesh(Graphics::MeshBuilder::CreateCircle(24.0f, 24));

    auto redTexture = textureManager.LoadTexture("Resources/red.png").value();
    auto greenTexture = textureManager.LoadTexture("Resources/green.png").value();
    auto purpleTexture = textureManager.LoadTexture("Resources/purple.png").value();
    m_RedTextureId = m_RenderSystem->CreateTexture(*redTexture);
    m_GreenTextureId = m_RenderSystem->CreateTexture(*greenTexture);
    m_PurpleTextureId = m_RenderSystem->CreateTexture(*purpleTexture);

    // Temporary: Create a few planets
    CreatePlanet(GetWorld(), Math::Vec3f{100.0f, 300.0f, 0.0f}, 500.0f, 24.0f, Math::Vec3f{5.0f, 0.0f, 0.0f}, PlanetColor::Red);
    CreatePlanet(GetWorld(), Math::Vec3f{600.0f, 400.0f, 0.0f}, 500.0f, 24.0f, Math::Vec3f{-1.0f, -2.0f, 0.0f}, PlanetColor::Green);
    CreatePlanet(GetWorld(), Math::Vec3f{900.0f, 800.0f, 0.0f}, 500.0f, 24.0f, Math::Vec3f{0.0f, 10.0f, 0.0f}, PlanetColor::Purple);

    GetWorld().AddSystem<GravitySystem>();
  }

  void OnImGui() override {}

  Scene::Entity CreatePlanet(Scene::World &world, const Math::Vec3f &position, f64 mass, f64 radius, Math::Vec2f velocity, PlanetColor color) {
    Scene::Entity planet = world.CreateEntity();
    world.AddComponent<Scene::Components::Transform>(planet, position);
    world.AddComponent<CelestialBody>(planet, CelestialBody{mass, radius, velocity});
    world.AddComponent<Trail>(planet, Trail{Vector<Math::Vec2f>{position}});

    switch (color) {
    case PlanetColor::Red:    world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMesh, m_RedTextureId); break;
    case PlanetColor::Green:  world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMesh, m_GreenTextureId); break;
    case PlanetColor::Purple: world.AddComponent<Scene::Components::Renderable>(planet, m_PlanetMesh, m_PurpleTextureId); break;
    }

    return planet;
  }

private:
  Graphics::Mesh m_PlanetMesh;
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