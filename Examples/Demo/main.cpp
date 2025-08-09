#include "Application/ClientApplication.hpp"
#include "Base/Profiler.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/Components/Renderable.hpp"
#include "Graphics/DefaultPass.hpp"
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

struct ObjectCreationInfo {
  Graphics::Mesh mesh;
  Id textureId;
  Math::Vec3f position;
  f32 rotation;
  Math::Vec2f size;
  Physics::BodyType bodyType;
};

Scene::Entity CreateObject(const ObjectCreationInfo &info, Scene::World &world, Physics::PhysicsSystem *physicsSystem) {
  Scene::Entity entity = world.CreateEntity();
  world.AddComponent<Scene::Components::Transform>(entity, info.position, info.rotation);
  world.AddComponent<Scene::Components::Renderable>(entity, info.mesh, info.textureId);

  auto [rb, boxcollider] = physicsSystem->CreateBoxBody({
      {info.position.x, info.position.y},
      {info.size.x,     info.size.y    },
      info.bodyType
  });
  world.AddComponent<Scene::Components::RigidBody>(entity, rb);
  world.AddComponent<Scene::Components::BoxCollider>(entity, boxcollider);

  return entity;
}

class Demo : public ClientApplication {
public:
  Demo() : ClientApplication(appInfo) {}

protected:
  void OnInitialize() override {
    m_RenderSystem->AddRenderPass<Graphics::DefaultPass>();

    m_PhysicsSystem = GetWorld().AddSystem<Physics::PhysicsSystem>(Physics::PhysicsSystemConfig{
        Math::Vec2f{0.0f, 9.81f},
        30.0f, 4
    });

    Graphics::RawMesh quadMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f});
    m_QuadMesh = m_RenderSystem->CreateMesh(quadMesh);

    Graphics::RawMesh circleMesh = Graphics::MeshBuilder::CreateCircle(20.0f, 16);
    m_CircleMesh = m_RenderSystem->CreateMesh(circleMesh);

    Graphics::RawMesh groundRawMesh = Graphics::MeshBuilder::CreateQuad({1000.0f, 50.0f}, Graphics::Colors::Green);
    Graphics::Mesh groundMesh = m_RenderSystem->CreateMesh(groundRawMesh);

    Graphics::RawMesh redBoxRawMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Red);
    Graphics::Mesh redBoxMesh = m_RenderSystem->CreateMesh(redBoxRawMesh);

    Graphics::RawMesh blueBoxRawMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Blue);
    Graphics::Mesh blueBoxMesh = m_RenderSystem->CreateMesh(blueBoxRawMesh);

    auto squareTexture = textureManager.LoadTexture("Resources/square.png").value();
    auto circleTexture = textureManager.LoadTexture("Resources/circle.png").value();

    m_WhitePixelTextureId = m_RenderSystem->CreateTexture(Graphics::Texture{1, 1, {0xFFFFFFFF}});
    m_SquareTextureId = m_RenderSystem->CreateTexture(*squareTexture);
    m_CircleTextureId = m_RenderSystem->CreateTexture(*circleTexture);

    ObjectCreationInfo groundInfo = {
        groundMesh, m_WhitePixelTextureId, Math::Vec3f{512.0f, 725.0f, 0.0f},
          0.1f, Math::Vec2f{1000.0f, 50.0f},
          Physics::BodyType::Static
    };
    CreateObject(groundInfo, GetWorld(), m_PhysicsSystem);

    ObjectCreationInfo redBoxInfo = {
        redBoxMesh, m_WhitePixelTextureId, Math::Vec3f{400.0f, 100.0f, 0.0f},
          0.0f, Math::Vec2f{40.0f, 40.0f},
          Physics::BodyType::Dynamic
    };
    CreateObject(redBoxInfo, GetWorld(), m_PhysicsSystem);

    ObjectCreationInfo blueBoxInfo = {
        blueBoxMesh, m_WhitePixelTextureId, Math::Vec3f{600.0f, 100.0f, 0.0f},
          0.0f, Math::Vec2f{40.0f, 40.0f},
          Physics::BodyType::Dynamic
    };
    m_Player = CreateObject(blueBoxInfo, GetWorld(), m_PhysicsSystem);
    GetWorld().AddComponent<PlayerComponent>(m_Player);

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
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      ObjectCreationInfo boxInfo = {
          m_QuadMesh, m_SquareTextureId, Math::Vec3f{x, y, 0.0f},
            0.0f, Math::Vec2f{40.0f, 40.0f},
            Physics::BodyType::Dynamic
      };
      CreateObject(boxInfo, GetWorld(), m_PhysicsSystem);
    }

    if (ImGui::Button("Spawn Circle")) {
      float x = static_cast<float>(rand() % 800 + 100);
      float y = static_cast<float>(rand() % 400 + 100);
      ObjectCreationInfo circleInfo = {
          m_CircleMesh, m_CircleTextureId, Math::Vec3f{x, y, 0.0f},
            0.1f, Math::Vec2f{20.0f, 20.0f},
            Physics::BodyType::Dynamic
      };
      CreateObject(circleInfo, GetWorld(), m_PhysicsSystem);
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