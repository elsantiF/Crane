#include "SDLGPURenderer.hpp"
#include "Base/Assert.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

namespace Crane::Graphics::SDLGPURenderer {
  void SDLGPURenderer::Initialize() {
    m_Context.gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
    if (!m_Context.gpuDevice) {
      Assert::Crash(std::format("Failed to create GPU device: {}", SDL_GetError()));
    }

    if (!SDL_ClaimWindowForGPUDevice(m_Context.gpuDevice, m_Window)) {
      Assert::Crash(std::format("Failed to claim window for GPU device: {}", SDL_GetError()));
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLGPU(m_Window);
    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.Device = m_Context.gpuDevice;
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_Context.gpuDevice, m_Window);
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&initInfo);
  }

  void SDLGPURenderer::Shutdown() {
    SDL_WaitForGPUIdle(m_Context.gpuDevice);
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();

    SDL_ReleaseWindowFromGPUDevice(m_Context.gpuDevice, m_Window);

    if (m_Context.gpuDevice) {
      SDL_DestroyGPUDevice(m_Context.gpuDevice);
      m_Context.gpuDevice = nullptr;
    }
  }

  void SDLGPURenderer::BeginImGuiFrame() {
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void SDLGPURenderer::EndImGuiFrame() {
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();
    SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(m_Context.gpuDevice);
    SDL_GPUTexture *swapchainTexture;
    SDL_AcquireGPUSwapchainTexture(commandBuffer, m_Window, &swapchainTexture, nullptr, nullptr);
    if (swapchainTexture) {
      ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

      SDL_GPUColorTargetInfo targetInfo = {};
      targetInfo.texture = swapchainTexture;
      targetInfo.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
      targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
      targetInfo.store_op = SDL_GPU_STOREOP_STORE;
      targetInfo.mip_level = 0;
      targetInfo.layer_or_depth_plane = 0;
      targetInfo.cycle = false;

      SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
      ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);
      SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(commandBuffer);
  }
}