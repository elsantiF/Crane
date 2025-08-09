#include "SDLGPURenderer.hpp"
#include "Base/Assert.hpp"
#include "Base/Profiler.hpp"
#include "Graphics/Primitives/Vertex.hpp"
#include "Graphics/RenderPass.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

static const SDL_GPULoadOp ConvertLoadOperation(Crane::Graphics::LoadOperation op) {
  switch (op) {
  case Crane::Graphics::LoadOperation::Clear:    return SDL_GPU_LOADOP_CLEAR;
  case Crane::Graphics::LoadOperation::DontCare: return SDL_GPU_LOADOP_DONT_CARE;
  case Crane::Graphics::LoadOperation::Load:     return SDL_GPU_LOADOP_LOAD;
  default:                                       return SDL_GPU_LOADOP_CLEAR;
  }
}

static const SDL_GPUStoreOp ConvertStoreOperation(Crane::Graphics::StoreOperation op) {
  switch (op) {
  case Crane::Graphics::StoreOperation::Store:    return SDL_GPU_STOREOP_STORE;
  case Crane::Graphics::StoreOperation::DontCare: return SDL_GPU_STOREOP_DONT_CARE;
  default:                                        return SDL_GPU_STOREOP_STORE;
  }
}

static const SDL_GPUSamplerAddressMode ConvertAddressMode(Crane::Graphics::AddressMode mode) {
  switch (mode) {
  case Crane::Graphics::AddressMode::Repeat:         return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  case Crane::Graphics::AddressMode::MirroredRepeat: return SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
  case Crane::Graphics::AddressMode::ClampToEdge:    return SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
  default:                                           return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
  }
}

static const SDL_GPUFilter ConvertFilterMode(Crane::Graphics::FilterMode mode) {
  switch (mode) {
  case Crane::Graphics::FilterMode::Nearest: return SDL_GPU_FILTER_NEAREST;
  case Crane::Graphics::FilterMode::Linear:  return SDL_GPU_FILTER_LINEAR;
  default:                                   return SDL_GPU_FILTER_LINEAR;
  }
}

static const SDL_GPUPrimitiveType ConvertPrimitiveType(Crane::Graphics::PrimitiveType type) {
  switch (type) {
  case Crane::Graphics::PrimitiveType::TriangleList:  return SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  case Crane::Graphics::PrimitiveType::TriangleStrip: return SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP;
  case Crane::Graphics::PrimitiveType::LineList:      return SDL_GPU_PRIMITIVETYPE_LINELIST;
  case Crane::Graphics::PrimitiveType::LineStrip:     return SDL_GPU_PRIMITIVETYPE_LINESTRIP;
  case Crane::Graphics::PrimitiveType::PointList:     return SDL_GPU_PRIMITIVETYPE_POINTLIST;
  default:                                            return SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  }
}

static const SDL_GPUVertexElementFormat ConvertVertexElementFormat(Crane::Graphics::VertexElementFormat format) {
  switch (format) {
  case Crane::Graphics::VertexElementFormat::Float1: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
  case Crane::Graphics::VertexElementFormat::Float2: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
  case Crane::Graphics::VertexElementFormat::Float3: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
  case Crane::Graphics::VertexElementFormat::Float4: return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
  case Crane::Graphics::VertexElementFormat::Int1:   return SDL_GPU_VERTEXELEMENTFORMAT_INT;
  case Crane::Graphics::VertexElementFormat::Int2:   return SDL_GPU_VERTEXELEMENTFORMAT_INT2;
  case Crane::Graphics::VertexElementFormat::Int3:   return SDL_GPU_VERTEXELEMENTFORMAT_INT3;
  case Crane::Graphics::VertexElementFormat::Int4:   return SDL_GPU_VERTEXELEMENTFORMAT_INT4;
  default:                                           return SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
  }
}

static const SDL_GPUShaderStage ConvertShaderStage(Crane::Graphics::ShaderType type) {
  switch (type) {
  case Crane::Graphics::ShaderType::Vertex:   return SDL_GPU_SHADERSTAGE_VERTEX;
  case Crane::Graphics::ShaderType::Fragment: return SDL_GPU_SHADERSTAGE_FRAGMENT;
  default:                                    return SDL_GPU_SHADERSTAGE_VERTEX;
  }
}

namespace Crane::Graphics::SDLGPURenderer {
  void SDLGPURenderer::Initialize() {
    PROFILE_SCOPE();
    m_Context.gpuDevice = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan");
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

  void SDLGPURenderer::EndFrame() {
    PROFILE_SCOPE();
    if (m_Context.commandBuffer) {
      SDL_SubmitGPUCommandBuffer(m_Context.commandBuffer);
      m_Context.commandBuffer = nullptr;
    }
  }

  void SDLGPURenderer::BeginRenderPass(const RenderPassConfig &config) {
    PROFILE_SCOPE();
    if (m_Context.swapchainTexture) {
      SDL_GPUColorTargetInfo targetInfo = {
          .texture = m_Context.swapchainTexture,
          .clear_color = SDL_FColor{config.clearColor.r, config.clearColor.g, config.clearColor.b, config.clearColor.a},
          .load_op = ConvertLoadOperation(config.loadOp),
          .store_op = ConvertStoreOperation(config.storeOp),
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

  Id SDLGPURenderer::CreateBufferImpl(BufferType type, size_t size, const void *data) {
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

  Id SDLGPURenderer::CreateShader(const ShaderCreateInfo &createInfo) {
    PROFILE_SCOPE();
    SDL_GPUShaderCreateInfo sdlCreateInfo = {
        .code_size = createInfo.size,
        .code = createInfo.source,
        .entrypoint = createInfo.entryPoint.c_str(),
        .format = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage = ConvertShaderStage(createInfo.type),
        .num_samplers = createInfo.numSamplers,
        .num_uniform_buffers = createInfo.numUniformBuffers,
    };

    SDL_GPUShader *shader = SDL_CreateGPUShader(m_Context.gpuDevice, &sdlCreateInfo);
    if (!shader) {
      Assert::Crash(std::format("Failed to create GPU shader: {}", SDL_GetError()));
    }

    SDLGPUShader gpuShader{.type = createInfo.type, .shader = shader};

    Id shaderId = m_Shaders.size() + 1;
    m_Shaders[shaderId] = gpuShader;
    Logger::Info("Created {} shader with ID: {}", (createInfo.type == ShaderType::Vertex ? "vertex" : "fragment"), shaderId);
    return shaderId;
  }

  Id SDLGPURenderer::CreateTexture(const Texture &texture) {
    PROFILE_SCOPE();
    SDL_GPUTextureCreateInfo createInfo = {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = texture.width,
        .height = texture.height,
        .layer_count_or_depth = 1,
        .num_levels = 1,
    };

    SDL_GPUTexture *gpuTexture = SDL_CreateGPUTexture(m_Context.gpuDevice, &createInfo);
    if (!gpuTexture) {
      Assert::Crash(std::format("Failed to create GPU texture: {}", SDL_GetError()));
    }

    if (!texture.data.empty()) {
      SDL_GPUTransferBufferCreateInfo transferInfo = {
          .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
          .size = static_cast<u32>(texture.data.size() * sizeof(u32)),
      };

      SDL_GPUTransferBuffer *transferBuffer = SDL_CreateGPUTransferBuffer(m_Context.gpuDevice, &transferInfo);
      if (!transferBuffer) {
        Assert::Crash(std::format("Failed to create GPU transfer buffer: {}", SDL_GetError()));
      }

      void *mappedData = SDL_MapGPUTransferBuffer(m_Context.gpuDevice, transferBuffer, false);
      std::memcpy(mappedData, texture.data.data(), transferInfo.size);
      SDL_UnmapGPUTransferBuffer(m_Context.gpuDevice, transferBuffer);

      SDL_GPUCommandBuffer *commandBuffer = SDL_AcquireGPUCommandBuffer(m_Context.gpuDevice);
      SDL_GPUCopyPass *copyPass = SDL_BeginGPUCopyPass(commandBuffer);

      SDL_GPUTextureTransferInfo textureTransferInfo = {
          .transfer_buffer = transferBuffer,
          .offset = 0,
      };

      SDL_GPUTextureRegion region = {
          .texture = gpuTexture,
          .w = texture.width,
          .h = texture.height,
          .d = 1,
      };

      SDL_UploadToGPUTexture(copyPass, &textureTransferInfo, &region, false);
      SDL_EndGPUCopyPass(copyPass);

      SDL_SubmitGPUCommandBuffer(commandBuffer);
      SDL_ReleaseGPUTransferBuffer(m_Context.gpuDevice, transferBuffer);
    }

    Id textureId = m_Textures.size() + 1;
    m_Textures[textureId] = gpuTexture;
    Logger::Info("Created texture with ID: {}", textureId);
    return textureId;
  }

  Id SDLGPURenderer::CreateSampler(const SamplerCreateInfo &info) {
    PROFILE_SCOPE();
    SDL_GPUSamplerCreateInfo createInfo = {
        .min_filter = ConvertFilterMode(info.minFilter),
        .mag_filter = ConvertFilterMode(info.magFilter),
        .address_mode_u = ConvertAddressMode(info.addressU),
        .address_mode_v = ConvertAddressMode(info.addressV),
        .address_mode_w = ConvertAddressMode(info.addressW),
    };

    SDL_GPUSampler *sampler = SDL_CreateGPUSampler(m_Context.gpuDevice, &createInfo);
    if (!sampler) {
      Assert::Crash(std::format("Failed to create GPU sampler: {}", SDL_GetError()));
    }

    Id samplerId = m_Samplers.size() + 1;
    m_Samplers[samplerId] = sampler;
    Logger::Info("Created sampler with ID: {}", samplerId);
    return samplerId;
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

    SDL_GPUVertexBufferDescription vertexBufferDesc[] = {
        {.slot = 0, .pitch = state.vertexSize, .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX},
    };

    // TODO: Define vertex attributes based on a vertex structure
    Vector<SDL_GPUVertexAttribute> vertexAttributes;

    for (const auto &attr : state.vertexDefinition) {
      SDL_GPUVertexAttribute gpuAttr = {};
      gpuAttr.location = attr.location;
      gpuAttr.buffer_slot = 0;
      gpuAttr.offset = attr.offset;
      gpuAttr.format = ConvertVertexElementFormat(attr.format);
      vertexAttributes.push_back(gpuAttr);
    }

    SDL_GPUVertexInputState vertexInputState = {
        .vertex_buffer_descriptions = vertexBufferDesc,
        .num_vertex_buffers = 1,
        .vertex_attributes = vertexAttributes.data(),
        .num_vertex_attributes = static_cast<u32>(vertexAttributes.size()),
    };

    SDL_GPUGraphicsPipelineCreateInfo pipelineInfo = {
        .vertex_shader = vertexShader,
        .fragment_shader = fragmentShader,
        .vertex_input_state = vertexInputState,
        .primitive_type = ConvertPrimitiveType(state.primitiveType),
        .rasterizer_state = rasterizerState,
        .target_info = {.color_target_descriptions = &colorTargetDesc, .num_color_targets = 1},
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

  void SDLGPURenderer::PushVertexUniformData(u32 slot, const void *data, size_t size) {
    PROFILE_SCOPE();
    if (!data || size == 0) {
      Logger::Error("Invalid data or size for uniform vertex data");
      return;
    }

    SDL_PushGPUVertexUniformData(m_Context.commandBuffer, slot, data, size);
  }

  void SDLGPURenderer::BindTexture(Id textureId, Id samplerId) {
    PROFILE_SCOPE();
    auto texIt = m_Textures.find(textureId);
    if (texIt == m_Textures.end()) {
      Logger::Error("Tried to bind non-existent texture with ID: {}", textureId);
      return;
    }

    auto sampIt = m_Samplers.find(samplerId);
    if (sampIt == m_Samplers.end()) {
      Logger::Error("Tried to bind non-existent sampler with ID: {}", samplerId);
      return;
    }

    SDL_GPUTexture *texture = texIt->second;
    SDL_GPUSampler *sampler = sampIt->second;

    SDL_GPUTextureSamplerBinding textureBinding = {
        .texture = texture,
        .sampler = sampler,
    };

    SDL_BindGPUFragmentSamplers(m_Context.renderPass, 0, &textureBinding, 1);
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