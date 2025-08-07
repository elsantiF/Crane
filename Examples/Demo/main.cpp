#include "Application/ClientApplication.hpp"
#include "Base/Profiler.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/Components/Renderable.hpp"
#include "Graphics/Primitives/Mesh.hpp"
#include "Graphics/TextureManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "PlayerComponent.hpp"
#include "PlayerSystem.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/RigidBody.hpp"
#include "Scene/Components/Transform.hpp"
#include <imgui.h>

using namespace Crane;

const ApplicationInfo appInfo = {
    "Crane Demo", {1,    0,   0   },
     {1600, 900, true}
};

static Graphics::TextureManager textureManager;

class Demo : public ClientApplication {
public:
  Demo() : ClientApplication(appInfo) {}

protected:
  void OnInitialize() override {
    m_PhysicsSystem = GetWorld().AddSystem<Physics::PhysicsSystem>(Physics::PhysicsSystemConfig{
        Math::Vec2f{0.0f, 9.81f},
        30.0f, 4
    });

    Graphics::RawMesh quadMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f});
    m_QuadMesh = m_RenderSystem->CreateMesh(quadMesh);

    Graphics::RawMesh circleMesh = Graphics::MeshBuilder::CreateCircle(20.0f, 16);
    m_CircleMesh = m_RenderSystem->CreateMesh(circleMesh);

    auto squareTexture = textureManager.LoadTexture("Resources/square.png").value();
    auto circleTexture = textureManager.LoadTexture("Resources/circle.png").value();

    m_WhitePixelTextureId = m_RenderSystem->CreateTexture(Graphics::Texture{1, 1, {0xFFFFFFFF}});
    m_SquareTextureId = m_RenderSystem->CreateTexture(*squareTexture);
    m_CircleTextureId = m_RenderSystem->CreateTexture(*circleTexture);

    Graphics::RawMesh groundRawMesh = Graphics::MeshBuilder::CreateQuad({1000.0f, 50.0f}, Graphics::Colors::Green);
    Graphics::Mesh groundMesh = m_RenderSystem->CreateMesh(groundRawMesh);

    Scene::Entity ground = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(ground, Math::Vec3f{512.0f, 725.0f, 0.0f}, 0.1f);
      GetWorld().AddComponent<Scene::Components::Renderable>(ground, groundMesh, m_WhitePixelTextureId);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {512,  700},
          {1000, 50 },
          Physics::BodyType::Static
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(ground, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(ground, boxcollider);
    }

    Graphics::RawMesh redBoxRawMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Red);
    Graphics::Mesh redBoxMesh = m_RenderSystem->CreateMesh(redBoxRawMesh);

    Scene::Entity box = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(box, Math::Vec3f{400.0f, 100.0f, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(box, redBoxMesh, m_WhitePixelTextureId);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {400, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(box, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(box, boxcollider);
    }

    Graphics::RawMesh blueBoxRawMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Blue);
    Graphics::Mesh blueBoxMesh = m_RenderSystem->CreateMesh(blueBoxRawMesh);

    m_Player = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(m_Player, Math::Vec3f{600.0f, 100.0f, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(m_Player, blueBoxMesh, m_WhitePixelTextureId);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {600, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(m_Player, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(m_Player, boxcollider);
      GetWorld().AddComponent<PlayerComponent>(m_Player);
    }

    GetWorld().AddSystem<PlayerSystem>(m_Player);
  }

  void OnImGui() override {
    PROFILE_SCOPE();
    ImGui::Begin("Stats");
    ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
    ImGui::Text("ESC to exit");
    ImGui::End();

    auto &registry = m_World->GetRegistry();
    Editor::EntityDisplay::DrawEntityList(registry);

    ImGui::Begin("Demo Controls");

    if (ImGui::Button("Spawn Box")) {
      Scene::Entity box = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      GetWorld().AddComponent<Scene::Components::Transform>(box, Math::Vec3f{x, y, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(box, m_QuadMesh, m_SquareTextureId);
      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {x,  y },
          {40, 40},
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(box, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(box, boxcollider);
    }

    if (ImGui::Button("Spawn Circle")) {
      Scene::Entity circle = GetWorld().CreateEntity();
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      GetWorld().AddComponent<Scene::Components::Transform>(circle, Math::Vec3f{x, y, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(circle, m_CircleMesh, m_CircleTextureId);
      auto [rb, circlecollider] = m_PhysicsSystem->CreateCircleBody({
          {x, y},
          20, Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(circle, rb);
      GetWorld().AddComponent<Scene::Components::CircleCollider>(circle, circlecollider);
    }

    ImGui::End();
  }

private:
  Scene::Entity m_Player;

  Graphics::Mesh m_QuadMesh;
  Graphics::Mesh m_CircleMesh;

  Id m_WhitePixelTextureId = 0;
  Id m_SquareTextureId = 0;
  Id m_CircleTextureId = 0;

  Physics::PhysicsSystem *m_PhysicsSystem;
};

int main() {
  Demo demo;
  demo.Initialize();
  demo.Run();
  return 0;
}