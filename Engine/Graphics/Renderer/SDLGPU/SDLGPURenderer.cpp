#include "SDLGPURenderer.hpp"
#include "Base/Assert.hpp"
#include "Base/Profiler.hpp"
#include "Graphics/Primitives/Color.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

namespace Crane::Graphics::SDLGPURenderer {
  void SDLGPURenderer::Initialize() {
    PROFILE_SCOPE();
    m_Context.gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, "vulkan");
    if (!m_Context.gpuDevice) {
      Assert::Crash(std::format("Failed to create GPU device: {}", SDL_GetError()));
    }

    if (!SDL_ClaimWindowForGPUDevice(m_Context.gpuDevice, m_Context.window)) {
      Assert::Crash(std::format("Failed to claim window for GPU device: {}", SDL_GetError()));
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLGPU(m_Context.window);
    ImGui_ImplSDLGPU3_InitInfo initInfo = {};
    initInfo.Device = m_Context.gpuDevice;
    initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(m_Context.gpuDevice, m_Context.window);
    initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
    ImGui_ImplSDLGPU3_Init(&initInfo);
  }

  void SDLGPURenderer::Shutdown() {
    PROFILE_SCOPE();
    SDL_WaitForGPUIdle(m_Context.gpuDevice);
    ImGui_ImplSDL3_Shutdown();
    ImGui_ImplSDLGPU3_Shutdown();
    ImGui::DestroyContext();

    SDL_ReleaseWindowFromGPUDevice(m_Context.gpuDevice, m_Context.window);

    if (m_Context.gpuDevice) {
      SDL_DestroyGPUDevice(m_Context.gpuDevice);
      m_Context.gpuDevice = nullptr;
    }
  }

  void SDLGPURenderer::BeginFrame() {
    PROFILE_SCOPE();
    m_Context.commandBuffer = SDL_AcquireGPUCommandBuffer(m_Context.gpuDevice);
    SDL_AcquireGPUSwapchainTexture(m_Context.commandBuffer, m_Context.window, &m_Context.swapchainTexture, nullptr, nullptr);
  }

  void SDLGPURenderer::BeginRenderPass() {
    PROFILE_SCOPE();
    Color clearColor = Colors::CLEAR_COLOR;
    if (m_Context.swapchainTexture) {
      SDL_GPUColorTargetInfo targetInfo = {
          .texture = m_Context.swapchainTexture,
          .clear_color = SDL_FColor{clearColor.r, clearColor.g, clearColor.b, clearColor.a},
          .load_op = SDL_GPU_LOADOP_CLEAR,
          .store_op = SDL_GPU_STOREOP_STORE,
          .cycle = false,
      };

      m_Context.renderPass = SDL_BeginGPURenderPass(m_Context.commandBuffer, &targetInfo, 1, nullptr);
    }
  }

  void SDLGPURenderer::EndRenderPass() {
    PROFILE_SCOPE();
    if (m_Context.renderPass) {
      SDL_EndGPURenderPass(m_Context.renderPass);
      m_Context.renderPass = nullptr;
    }
  }

  void SDLGPURenderer::SubmitCommandBuffer() {
    PROFILE_SCOPE();
    if (m_Context.commandBuffer) {
      SDL_SubmitGPUCommandBuffer(m_Context.commandBuffer);
      m_Context.commandBuffer = nullptr;
    }
  }

  Id SDLGPURenderer::CreateBuffer(BufferType type, size_t size, const void *data) {
    PROFILE_SCOPE();
    SDL_GPUBufferCreateInfo createInfo = {};
    createInfo.size = size;
    switch (type) {
    case BufferType::Vertex: createInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX; break;
    case BufferType::Index:  createInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX; break;
    default:                 Assert::Crash("Unsupported buffer type"); return 0;
    }

    SDL_GPUBuffer *buffer = SDL_CreateGPUBuffer(m_Context.gpuDevice, &createInfo);
    if (!buffer) {
      Assert::Crash(std::format("Failed to create GPU buffer: {}", SDL_GetError()));
    }

    if (data) {
      SDL_GPUTransferBufferCreateInfo transferInfo = {};
      transferInfo.size = size;
      transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      SDL_GPUTransferBuffer *transferBuffer = SDL_CreateGPUTransferBuffer(m_Context.gpuDevice, &transferInfo);
      if (!transferBuffer) {
        Assert::Crash(std::format("Failed to create GPU transfer buffer: {}", SDL_GetError()));
      }

      void *mappedData = SDL_MapGPUTransferBuffer(m_Context.gpuDevice, transferBuffer, false);
      std::memcpy(mappedData, data, size);
      SDL_UnmapGPUTransferBuffer(m_Context.gpuDevice, transferBuffer);

      SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(m_Context.gpuDevice);
      SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);
      SDL_GPUTransferBufferLocation location = {.transfer_buffer = transferBuffer, .offset = 0};
      SDL_GPUBufferRegion region = {.buffer = buffer, .offset = 0, .size = static_cast<u32>(size)};

      SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
      SDL_EndGPUCopyPass(copyPass);
      SDL_SubmitGPUCommandBuffer(commandBuffer);
      SDL_ReleaseGPUTransferBuffer(m_Context.gpuDevice, transferBuffer);
    }

    SDLGPUBuffer gpuBuffer{.type = type, .buffer = buffer, .indexSize = SDL_GPU_INDEXELEMENTSIZE_32BIT};

    Id bufferId = m_Buffers.size() + 1;
    m_Buffers[bufferId] = gpuBuffer;

    Logger::Info("Created {} buffer with ID: {}", (type == BufferType::Vertex ? "vertex" : "index"), bufferId);
    return bufferId;
  }

  Id SDLGPURenderer::CreateShader(const ShaderType shaderType, const u8 *source, const u32 size, const String &entryPoint) {
    PROFILE_SCOPE();
    SDL_GPUShaderCreateInfo createInfo = {
        .code_size = size,
        .code = source,
        .entrypoint = entryPoint.c_str(),
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = (shaderType == ShaderType::Vertex) ? SDL_GPU_SHADERSTAGE_VERTEX : SDL_GPU_SHADERSTAGE_FRAGMENT,
    };

    SDL_GPUShader *shader = SDL_CreateGPUShader(m_Context.gpuDevice, &createInfo);
    if (!shader) {
      Assert::Crash(std::format("Failed to create GPU shader: {}", SDL_GetError()));
    }

    SDLGPUShader gpuShader{.type = shaderType, .shader = shader};

    Id shaderId = m_Shaders.size() + 1;
    m_Shaders[shaderId] = gpuShader;
    Logger::Info("Created {} shader with ID: {}", (shaderType == ShaderType::Vertex ? "vertex" : "fragment"), shaderId);
    return shaderId;
  }

  Id SDLGPURenderer::CreatePipeline(const PipelineCreateInfo &state) {
    PROFILE_SCOPE();
    SDL_GPUShader *vertexShader = m_Shaders[state.vertexShaderId].shader;
    SDL_GPUShader *fragmentShader = m_Shaders[state.fragmentShaderId].shader;
    if (!vertexShader || !fragmentShader) {
      Assert::Crash("Cannot create pipeline without valid vertex and fragment shaders");
      return 0;
    }

    SDL_GPUColorTargetDescription colorTargetDesc = {
        .format = SDL_GetGPUSwapchainTextureFormat(m_Context.gpuDevice, m_Context.window),
    };

    SDL_GPURasterizerState rasterizerState = {.fill_mode = SDL_GPU_FILLMODE_FILL};

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        .rasterizer_state = rasterizerState,
        .target_info = {.color_target_descriptions = &colorTargetDesc, .num_color_targets = 1}
    };

    SDL_GPUGraphicsPipeline *pipeline = SDL_CreateGPUGraphicsPipeline(m_Context.gpuDevice, &pipelineInfo);
    if (!pipeline) {
      Assert::Crash(std::format("Failed to create GPU graphics pipeline: {}", SDL_GetError()));
      return 0;
    }

    Id pipelineId = m_Pipelines.size() + 1;
    m_Pipelines[pipelineId] = pipeline;

    Logger::Info("Created graphics pipeline with ID: {}", pipelineId);
    return pipelineId;
  }

  void SDLGPURenderer::BindPipeline(Id pipelineId) {
    PROFILE_SCOPE();
    auto it = m_Pipelines.find(pipelineId);
    if (it == m_Pipelines.end()) {
      Logger::Error("Tried to bind non-existent pipeline with ID: {}", pipelineId);
      return;
    }

    SDL_GPUGraphicsPipeline *pipeline = it->second;
    if (!pipeline) {
      Logger::Error("Pipeline with ID {} is null", pipelineId);
      return;
    }

    SDL_BindGPUGraphicsPipeline(m_Context.renderPass, pipeline);
  }

  void SDLGPURenderer::BindBuffer(Id bufferId, size_t offset) {
    PROFILE_SCOPE();
    auto it = m_Buffers.find(bufferId);
    if (it == m_Buffers.end()) {
      Logger::Error("Tried to bind non-existent buffer with ID: {}", bufferId);
      return;
    }

    const SDLGPUBuffer &gpuBuffer = it->second;
    const SDL_GPUBufferBinding binding = {
        .buffer = gpuBuffer.buffer,
        .offset = 0,
    };

    switch (gpuBuffer.type) {
    case BufferType::Vertex: {
      SDL_BindGPUVertexBuffers(m_Context.renderPass, 0, &binding, 1);
      break;
    }
    case BufferType::Index: {
      SDL_BindGPUIndexBuffer(m_Context.renderPass, &binding, gpuBuffer.indexSize);
      break;
    }
    }
  }

  void SDLGPURenderer::Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex) {
    PROFILE_SCOPE();
    if (!m_Context.renderPass) {
      return;
    }

    SDL_DrawGPUPrimitives(m_Context.renderPass, vertexCount, instanceCount, firstVertex, 0);
  }

  void SDLGPURenderer::DrawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex) {
    PROFILE_SCOPE();
    if (!m_Context.renderPass) {
      return;
    }

    SDL_DrawGPUIndexedPrimitives(m_Context.renderPass, indexCount, instanceCount, firstIndex, 0, 0);
  }

  void SDLGPURenderer::BeginImGuiFrame() {
    ImGui_ImplSDLGPU3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void SDLGPURenderer::EndImGuiFrame() {
    ImGui::Render();
    ImDrawData *drawData = ImGui::GetDrawData();
    if (m_Context.swapchainTexture) {
      ImGui_ImplSDLGPU3_PrepareDrawData(drawData, m_Context.commandBuffer);

      SDL_GPUColorTargetInfo targetInfo = {};
      targetInfo.texture = m_Context.swapchainTexture;
      targetInfo.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
      targetInfo.load_op = SDL_GPU_LOADOP_LOAD;
      targetInfo.store_op = SDL_GPU_STOREOP_STORE;
      targetInfo.mip_level = 0;
      targetInfo.layer_or_depth_plane = 0;
      targetInfo.cycle = false;

      SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(m_Context.commandBuffer, &targetInfo, 1, nullptr);
      ImGui_ImplSDLGPU3_RenderDrawData(drawData, m_Context.commandBuffer, renderPass);
      SDL_EndGPURenderPass(renderPass);
    }
  }
}