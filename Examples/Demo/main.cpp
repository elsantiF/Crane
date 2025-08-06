#include "Application/ClientApplication.hpp"
#include "Base/Profiler.hpp"
#include "Editor/EntityDisplay.hpp"
#include "Graphics/Primitives/Mesh.hpp"
#include "Graphics/TextureManager.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "PlayerComponent.hpp"
#include "PlayerSystem.hpp"
#include "Scene/Components/BoxCollider.hpp"
#include "Scene/Components/Renderable.hpp"
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

    Graphics::Mesh quadMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f});
    Graphics::Mesh circleMesh = Graphics::MeshBuilder::CreateCircle(20.0f, 16);

    m_QuadVertexBufferId = m_Renderer->CreateBuffer<const Graphics::SVertex2>(Graphics::BufferType::Vertex, quadMesh.vertices);
    m_QuadIndexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, quadMesh.indices);
    m_QuadIndexCount = quadMesh.indices.size();

    m_CircleVertexBufferId = m_Renderer->CreateBuffer<const Graphics::SVertex2>(Graphics::BufferType::Vertex, circleMesh.vertices);
    m_CircleIndexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, circleMesh.indices);
    m_CircleIndexCount = circleMesh.indices.size();

    auto squareTexture = textureManager.LoadTexture("Resources/square.png").value();
    auto circleTexture = textureManager.LoadTexture("Resources/circle.png").value();

    m_WhitePixelTextureId = m_Renderer->CreateTexture(Graphics::Texture{1, 1, {0xFFFFFFFF}});
    m_SquareTextureId = m_Renderer->CreateTexture(*squareTexture);
    m_CircleTextureId = m_Renderer->CreateTexture(*circleTexture);

    Graphics::Mesh groundMesh = Graphics::MeshBuilder::CreateQuad({1000.0f, 50.0f}, Graphics::Colors::Green);
    u32 groundMeshIndexCount = groundMesh.indices.size();
    Id groundMeshVertexBufferId = m_Renderer->CreateBuffer<const Graphics::SVertex2>(Graphics::BufferType::Vertex, groundMesh.vertices);
    Id groundIndexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, groundMesh.indices);
    Scene::Entity ground = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(ground, Math::Vec3f{512.0f, 725.0f, 0.0f}, 0.1f);
      GetWorld().AddComponent<Scene::Components::Renderable>(ground, groundMeshVertexBufferId, groundIndexBufferId, m_WhitePixelTextureId,
                                                             groundMeshIndexCount);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {512,  700},
          {1000, 50 },
          Physics::BodyType::Static
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(ground, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(ground, boxcollider);
    }

    Graphics::Mesh redBoxMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Red);
    Id redBoxVertexBufferId = m_Renderer->CreateBuffer<const Graphics::SVertex2>(Graphics::BufferType::Vertex, redBoxMesh.vertices);
    Id redBoxIndexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, redBoxMesh.indices);

    u32 redBoxIndexCount = redBoxMesh.indices.size();
    Scene::Entity box = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(box, Math::Vec3f{400.0f, 100.0f, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(box, redBoxVertexBufferId, redBoxIndexBufferId, m_WhitePixelTextureId, redBoxIndexCount);

      auto [rb, boxcollider] = m_PhysicsSystem->CreateBoxBody({
          {400, 100},
          {40,  40 },
          Physics::BodyType::Dynamic
      });
      GetWorld().AddComponent<Scene::Components::RigidBody>(box, rb);
      GetWorld().AddComponent<Scene::Components::BoxCollider>(box, boxcollider);
    }

    Graphics::Mesh blueBoxMesh = Graphics::MeshBuilder::CreateQuad({40.0f, 40.0f}, Graphics::Colors::Blue);
    Id blueBoxVertexBufferId = m_Renderer->CreateBuffer<const Graphics::SVertex2>(Graphics::BufferType::Vertex, blueBoxMesh.vertices);
    Id blueBoxIndexBufferId = m_Renderer->CreateBuffer<const u32>(Graphics::BufferType::Index, blueBoxMesh.indices);

    u32 blueBoxIndexCount = blueBoxMesh.indices.size();

    m_Player = GetWorld().CreateEntity();
    {
      GetWorld().AddComponent<Scene::Components::Transform>(m_Player, Math::Vec3f{600.0f, 100.0f, 0.0f});
      GetWorld().AddComponent<Scene::Components::Renderable>(m_Player, blueBoxVertexBufferId, blueBoxIndexBufferId, m_WhitePixelTextureId,
                                                             blueBoxIndexCount);
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
      GetWorld().AddComponent<Scene::Components::Renderable>(box, m_QuadVertexBufferId, m_QuadIndexBufferId, m_SquareTextureId, m_QuadIndexCount);
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
      GetWorld().AddComponent<Scene::Components::Renderable>(circle, m_CircleVertexBufferId, m_CircleIndexBufferId, m_CircleTextureId,
                                                             m_CircleIndexCount);
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

  Id m_QuadVertexBufferId = 0;
  Id m_QuadIndexBufferId = 0;
  Id m_CircleVertexBufferId = 0;
  Id m_CircleIndexBufferId = 0;
  u32 m_QuadIndexCount = 0;
  u32 m_CircleIndexCount = 0;

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