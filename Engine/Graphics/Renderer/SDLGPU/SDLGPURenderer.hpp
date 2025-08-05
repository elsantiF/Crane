#pragma once

#include "Base/Types.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include <SDL3/SDL_gpu.h>

namespace Crane::Graphics::SDLGPURenderer {
  class SDLGPURenderer : public IRenderer {
  public:
    SDLGPURenderer(SDL_Window *window) : m_Context{window, nullptr} {}
    ~SDLGPURenderer() override = default;

    void Initialize() override;
    void Shutdown() override;

    // Command buffer management
    void BeginFrame() override;
    void EndFrame() override;
    void BeginRenderPass(Id renderPassId) override {};
    void EndRenderPass() override {};
    void SubmitCommandBuffer() override;

    // Resource management
    Id CreateBuffer(BufferType type, size_t size, const void *data = nullptr) override {
      return 0;
    };
    void UpdateBuffer(Id bufferId, size_t offset, size_t size, const void *data) override {};
    void DestroyBuffer(Id bufferId) override {};

    Id CreateShader(const String &source, const String &entryPoint = "main") override {
      return 0;
    };
    void DestroyShader(Id shaderId) override {};

    Id CreateTexture(const Texture &texture) override {
      return 0;
    };
    void DestroyTexture(Id textureId) override {};

    Id CreatePipeline(const PipelineState &state) override {
      return 0;
    };
    void DestroyPipeline(Id pipelineId) override {};

    Id CreateRenderPass() override {
      return 0;
    };
    void DestroyRenderPass(Id renderPassId) override {};

    // Drawing commands
    void BindPipeline(Id pipelineId) override {};
    void BindVertexBuffer(Id bufferId, size_t offset = 0) override {};
    void BindIndexBuffer(Id bufferId, size_t offset = 0) override {};
    void BindTexture(Id textureId, size_t slot = 0) override {};

    void Draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0) override {};
    void DrawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0) override {};

    // ImGui integration
    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    String GetName() const override {
      return "SDLGPURenderer";
    }

  private:
    struct SDLGPUContext {
      SDL_Window *window = nullptr;
      SDL_GPUDevice *gpuDevice = nullptr;
      SDL_GPUCommandBuffer *commandBuffer = nullptr;
      SDL_GPUTexture *swapchainTexture = nullptr;
      SDL_GPURenderPass *renderPass = nullptr;
    } m_Context;

    UnorderedMap<Id, SDL_GPUBuffer *> m_Buffers;
    UnorderedMap<Id, SDL_GPUTexture *> m_Textures;
    UnorderedMap<Id, SDL_GPUShader *> m_Shaders;
    UnorderedMap<Id, SDL_GPURenderPass *> m_RenderPasses;
  };
}