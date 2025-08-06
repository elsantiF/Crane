#pragma once

#include "Base/Types.hpp"
#include "Primitives/Texture.hpp"

namespace Crane::Graphics {
  enum class BufferType { Vertex, Index };

  struct PipelineCreateInfo {
    Id vertexShaderId = 0;
    Id fragmentShaderId = 0;
    Id renderPassId = 0;
  };

  enum class ShaderType { Vertex, Fragment };

  class IRenderer {
  public:
    virtual ~IRenderer() = default;

    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    // Command buffer management
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void BeginRenderPass() = 0;
    virtual void EndRenderPass() = 0;
    virtual void SubmitCommandBuffer() = 0; // TODO: This is temporary

    // Resource management
    virtual Id CreateBuffer(BufferType type, size_t size, const void *data = nullptr) = 0;
    virtual void UpdateBuffer(Id bufferId, size_t offset, size_t size, const void *data) = 0;
    virtual void DestroyBuffer(Id bufferId) = 0;

    virtual Id CreateShader(const ShaderType shaderType, const u8 *source, const u32 size, const String &entryPoint = "main") = 0;
    virtual void DestroyShader(Id shaderId) = 0;

    virtual Id CreateTexture(const Texture &texture) = 0;
    virtual void DestroyTexture(Id textureId) = 0;

    virtual Id CreatePipeline(const PipelineCreateInfo &state) = 0;
    virtual void DestroyPipeline(Id pipelineId) = 0;

    virtual Id CreateRenderPass() = 0;
    virtual void DestroyRenderPass(Id renderPassId) = 0;

    // Drawing commands
    virtual void BindPipeline(Id pipelineId) = 0;
    virtual void BindBuffer(Id bufferId, size_t offset = 0) = 0;
    virtual void BindTexture(Id textureId, size_t slot = 0) = 0;

    virtual void Draw(u32 vertexCount, u32 instanceCount = 1, u32 firstVertex = 0) = 0;
    virtual void DrawIndexed(u32 indexCount, u32 instanceCount = 1, u32 firstIndex = 0) = 0;

    // ImGui integration
    virtual void BeginImGuiFrame() = 0;
    virtual void EndImGuiFrame() = 0;

    virtual String GetName() const = 0;
  };
}