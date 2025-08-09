#pragma once

#include "Base/Types.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include <SDL3/SDL_gpu.h>

namespace Crane::Graphics::SDLGPURenderer {
  struct SDLGPUBuffer {
    BufferType type;
    SDL_GPUBuffer *buffer;
    SDL_GPUIndexElementSize indexSize;
  };

  struct SDLGPUShader {
    ShaderType type;
    SDL_GPUShader *shader;
  };

  class SDLGPURenderer : public IRenderer {
  public:
    SDLGPURenderer(SDL_Window *window) : m_Context{window, nullptr} {}
    ~SDLGPURenderer() override = default;

    void Initialize() override;
    void Shutdown() override;

    // Command buffer management
    void BeginFrame() override;
    void EndFrame() override;
    void BeginRenderPass(const RenderPassConfig &config) override;
    void EndRenderPass() override;

    // Resource management
    void UpdateBuffer(Id bufferId, size_t offset, size_t size, const void *data) override {};
    void DestroyBuffer(Id bufferId) override {};

    Id CreateShader(const ShaderCreateInfo &createInfo) override;
    void DestroyShader(Id shaderId) override {};

    Id CreateTexture(const Texture &texture) override;
    void DestroyTexture(Id textureId) override {};

    Id CreateSampler(const SamplerCreateInfo &info) override;
    void DestroySampler(Id samplerId) override {};

    Id CreatePipeline(const PipelineCreateInfo &state) override;
    void DestroyPipeline(Id pipelineId) override {};

    // Drawing commands
    void BindPipeline(Id pipelineId) override;
    void BindBuffer(Id bufferId, size_t offset = 0) override;
    void BindTexture(Id textureId, Id samplerId) override;

    void PushVertexUniformData(u32 slot, const void *data, size_t size) override;

    void Draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0) override;
    void DrawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0) override;

    // ImGui integration
    void BeginImGuiFrame() override;
    void EndImGuiFrame() override;

    String GetName() const override {
      return "SDLGPURenderer";
    }

  protected:
    virtual Id CreateBufferImpl(BufferType type, size_t size, const void *data) override;

  private:
    struct SDLGPUContext {
      SDL_Window *window = nullptr;
      SDL_GPUDevice *gpuDevice = nullptr;
      SDL_GPUCommandBuffer *commandBuffer = nullptr;
      SDL_GPUTexture *swapchainTexture = nullptr;
      SDL_GPURenderPass *renderPass = nullptr;
    } m_Context;

    UnorderedMap<Id, SDLGPUBuffer> m_Buffers;
    UnorderedMap<Id, SDL_GPUTexture *> m_Textures;
    UnorderedMap<Id, SDL_GPUSampler *> m_Samplers;
    UnorderedMap<Id, SDLGPUShader> m_Shaders;
    UnorderedMap<Id, SDL_GPUGraphicsPipeline *> m_Pipelines;
  };
}