#include "ClientApplication.hpp"
#include "Base/Assert.hpp"
#include "Base/Logger.hpp"
#include "Base/Profiler.hpp"
#include "Core/Scene/Components/Renderable.hpp"
#include "Core/Scene/Components/Transform.hpp"
#include "Events/Events.hpp"
#include "Graphics/Renderer/SDLGPU/SDLGPURenderer.hpp"
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL.h>

const u32 vertexShaderSpv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x00000028, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
    0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0007000f, 0x00000000, 0x00000004, 0x6e69616d, 0x00000000, 0x00000019, 0x0000001d, 0x00030003,
    0x00000002, 0x000001c2, 0x000a0004, 0x475f4c47, 0x4c474f4f, 0x70635f45, 0x74735f70, 0x5f656c79, 0x656e696c, 0x7269645f, 0x69746365, 0x00006576,
    0x00080004, 0x475f4c47, 0x4c474f4f, 0x6e695f45, 0x64756c63, 0x69645f65, 0x74636572, 0x00657669, 0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
    0x00050005, 0x0000000c, 0x69736f70, 0x6e6f6974, 0x00000073, 0x00060005, 0x00000017, 0x505f6c67, 0x65567265, 0x78657472, 0x00000000, 0x00060006,
    0x00000017, 0x00000000, 0x505f6c67, 0x7469736f, 0x006e6f69, 0x00070006, 0x00000017, 0x00000001, 0x505f6c67, 0x746e696f, 0x657a6953, 0x00000000,
    0x00070006, 0x00000017, 0x00000002, 0x435f6c67, 0x4470696c, 0x61747369, 0x0065636e, 0x00070006, 0x00000017, 0x00000003, 0x435f6c67, 0x446c6c75,
    0x61747369, 0x0065636e, 0x00030005, 0x00000019, 0x00000000, 0x00060005, 0x0000001d, 0x565f6c67, 0x65747265, 0x646e4978, 0x00007865, 0x00030047,
    0x00000017, 0x00000002, 0x00050048, 0x00000017, 0x00000000, 0x0000000b, 0x00000000, 0x00050048, 0x00000017, 0x00000001, 0x0000000b, 0x00000001,
    0x00050048, 0x00000017, 0x00000002, 0x0000000b, 0x00000003, 0x00050048, 0x00000017, 0x00000003, 0x0000000b, 0x00000004, 0x00040047, 0x0000001d,
    0x0000000b, 0x0000002a, 0x00020013, 0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007,
    0x00000006, 0x00000002, 0x00040015, 0x00000008, 0x00000020, 0x00000000, 0x0004002b, 0x00000008, 0x00000009, 0x00000003, 0x0004001c, 0x0000000a,
    0x00000007, 0x00000009, 0x00040020, 0x0000000b, 0x00000006, 0x0000000a, 0x0004003b, 0x0000000b, 0x0000000c, 0x00000006, 0x0004002b, 0x00000006,
    0x0000000d, 0x00000000, 0x0004002b, 0x00000006, 0x0000000e, 0xbf000000, 0x0005002c, 0x00000007, 0x0000000f, 0x0000000d, 0x0000000e, 0x0004002b,
    0x00000006, 0x00000010, 0x3f000000, 0x0005002c, 0x00000007, 0x00000011, 0x00000010, 0x00000010, 0x0005002c, 0x00000007, 0x00000012, 0x0000000e,
    0x00000010, 0x0006002c, 0x0000000a, 0x00000013, 0x0000000f, 0x00000011, 0x00000012, 0x00040017, 0x00000014, 0x00000006, 0x00000004, 0x0004002b,
    0x00000008, 0x00000015, 0x00000001, 0x0004001c, 0x00000016, 0x00000006, 0x00000015, 0x0006001e, 0x00000017, 0x00000014, 0x00000006, 0x00000016,
    0x00000016, 0x00040020, 0x00000018, 0x00000003, 0x00000017, 0x0004003b, 0x00000018, 0x00000019, 0x00000003, 0x00040015, 0x0000001a, 0x00000020,
    0x00000001, 0x0004002b, 0x0000001a, 0x0000001b, 0x00000000, 0x00040020, 0x0000001c, 0x00000001, 0x0000001a, 0x0004003b, 0x0000001c, 0x0000001d,
    0x00000001, 0x00040020, 0x0000001f, 0x00000006, 0x00000007, 0x0004002b, 0x00000006, 0x00000022, 0x3f800000, 0x00040020, 0x00000026, 0x00000003,
    0x00000014, 0x00050036, 0x00000002, 0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0003003e, 0x0000000c, 0x00000013, 0x0004003d,
    0x0000001a, 0x0000001e, 0x0000001d, 0x00050041, 0x0000001f, 0x00000020, 0x0000000c, 0x0000001e, 0x0004003d, 0x00000007, 0x00000021, 0x00000020,
    0x00050051, 0x00000006, 0x00000023, 0x00000021, 0x00000000, 0x00050051, 0x00000006, 0x00000024, 0x00000021, 0x00000001, 0x00070050, 0x00000014,
    0x00000025, 0x00000023, 0x00000024, 0x0000000d, 0x00000022, 0x00050041, 0x00000026, 0x00000027, 0x00000019, 0x0000001b, 0x0003003e, 0x00000027,
    0x00000025, 0x000100fd, 0x00010038};

const u32 fragmentShaderSpv[] = {
    0x07230203, 0x00010000, 0x000d000b, 0x0000000d, 0x00000000, 0x00020011, 0x00000001, 0x0006000b, 0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
    0x00000000, 0x0003000e, 0x00000000, 0x00000001, 0x0006000f, 0x00000004, 0x00000004, 0x6e69616d, 0x00000000, 0x00000009, 0x00030010, 0x00000004,
    0x00000007, 0x00030003, 0x00000002, 0x000001c2, 0x000a0004, 0x475f4c47, 0x4c474f4f, 0x70635f45, 0x74735f70, 0x5f656c79, 0x656e696c, 0x7269645f,
    0x69746365, 0x00006576, 0x00080004, 0x475f4c47, 0x4c474f4f, 0x6e695f45, 0x64756c63, 0x69645f65, 0x74636572, 0x00657669, 0x00040005, 0x00000004,
    0x6e69616d, 0x00000000, 0x00050005, 0x00000009, 0x67617266, 0x6f6c6f43, 0x00000072, 0x00040047, 0x00000009, 0x0000001e, 0x00000000, 0x00020013,
    0x00000002, 0x00030021, 0x00000003, 0x00000002, 0x00030016, 0x00000006, 0x00000020, 0x00040017, 0x00000007, 0x00000006, 0x00000004, 0x00040020,
    0x00000008, 0x00000003, 0x00000007, 0x0004003b, 0x00000008, 0x00000009, 0x00000003, 0x0004002b, 0x00000006, 0x0000000a, 0x3f800000, 0x0004002b,
    0x00000006, 0x0000000b, 0x00000000, 0x0007002c, 0x00000007, 0x0000000c, 0x0000000a, 0x0000000b, 0x0000000b, 0x0000000a, 0x00050036, 0x00000002,
    0x00000004, 0x00000000, 0x00000003, 0x000200f8, 0x00000005, 0x0003003e, 0x00000009, 0x0000000c, 0x000100fd, 0x00010038};

namespace Crane {
  void ClientApplication::PlatformInitialize() {
    PROFILE_SCOPE();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      Assert::Crash("Failed to initialize SDL");
    }

    m_Window = MakeScope<Graphics::SDLWindow>(m_AppInfo.appName, m_AppInfo.window.width, m_AppInfo.window.height);
    m_Renderer = MakeScope<Graphics::SDLGPURenderer::SDLGPURenderer>(m_Window->GetHandle());
    m_Renderer->Initialize();

    Id vertexShaderId =
        m_Renderer->CreateShader(Graphics::ShaderType::Vertex, reinterpret_cast<const u8 *>(vertexShaderSpv), sizeof(vertexShaderSpv));
    Id fragmentShaderId =
        m_Renderer->CreateShader(Graphics::ShaderType::Fragment, reinterpret_cast<const u8 *>(fragmentShaderSpv), sizeof(fragmentShaderSpv));

    Graphics::PipelineCreateInfo pipelineInfo;
    pipelineInfo.vertexShaderId = vertexShaderId;
    pipelineInfo.fragmentShaderId = fragmentShaderId;

    m_RenderPipelineId = m_Renderer->CreatePipeline(pipelineInfo);
  }

  void ClientApplication::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL3_ProcessEvent(&event);

      switch (event.type) {
      case SDL_EVENT_QUIT: m_Running = false; break;
      case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_ESCAPE) {
          m_Running = false;
        }

        if (!event.key.repeat) {
          m_World->GetDispatcher().trigger(Events::KeyDown{event.key.key});
        }
        break;
      case SDL_EVENT_KEY_UP:
        if (!event.key.repeat) {
          m_World->GetDispatcher().trigger(Events::KeyUp{event.key.key});
        }
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: {
        auto position = Math::Vec2f(event.button.x, event.button.y);
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:   m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Left}); break;
        case SDL_BUTTON_RIGHT:  m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Right}); break;
        case SDL_BUTTON_MIDDLE: m_World->GetDispatcher().trigger(Events::MouseClick{position, Events::MouseButton::Middle}); break;
        default:
          // TODO: Handle other mouse buttons if needed
          break;
        }
        break;
      }
      }
    }

    m_World->GetDispatcher().update();
  }

  void ClientApplication::Render() {
    PROFILE_SCOPE();
    m_Renderer->BeginFrame();
    m_Renderer->BeginRenderPass();
    m_Renderer->BindPipeline(m_RenderPipelineId);
    auto &registry = m_World->GetRegistry();
    auto view = registry.view<Scene::Components::Transform, Scene::Components::Renderable>();

    for (auto entity : view) {
      const auto &[transform, renderable] = view.get<Scene::Components::Transform, Scene::Components::Renderable>(entity);

      m_Renderer->BindTexture(renderable.textureId);
      m_Renderer->BindBuffer(renderable.vertexBufferId);
      m_Renderer->BindBuffer(renderable.indexBufferId);
      m_Renderer->Draw(3);
    }

    m_Renderer->EndRenderPass();
    m_Renderer->BeginImGuiFrame();
    OnImGui();
    m_Renderer->EndImGuiFrame();
    m_Renderer->EndFrame();
  }

  void ClientApplication::Cleanup() {
    m_Renderer.reset();
    m_Window.reset();
    SDL_Quit();
    Logger::Info("Application cleaned up");
    Logger::Shutdown();
  }
}