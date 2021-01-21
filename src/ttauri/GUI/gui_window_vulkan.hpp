// Copyright 2019 Pokitec
// All rights reserved.

#pragma once

#include "gui_window.hpp"
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
#include <optional>

namespace tt {
class gui_device_vulkan;

namespace PipelineImage {
class PipelineImage;
}
namespace PipelineFlat {
class PipelineFlat;
}
namespace PipelineBox {
class PipelineBox;
}
namespace PipelineSDF {
class PipelineSDF;
}
namespace PipelineToneMapper {
class PipelineToneMapper;
}

class gui_window_vulkan : public gui_window {
public:
    vk::SurfaceKHR intrinsic;

    vk::SwapchainKHR swapchain;

    static constexpr uint32_t defaultNumberOfSwapchainImages = 2;

    int nrSwapchainImages;
    vk::Extent2D swapchainImageExtent;
    vk::SurfaceFormatKHR swapchainImageFormat;
    std::vector<vk::Image> swapchainImages;
    std::vector<vk::ImageView> swapchainImageViews;
    std::vector<vk::Framebuffer> swapchainFramebuffers;
    std::vector<aarect> swapchainRedrawRectangle;

    static const vk::Format depthImageFormat = vk::Format::eD32Sfloat;
    VmaAllocation depthImageAllocation;
    vk::Image depthImage;
    vk::ImageView depthImageView;

    static const vk::Format colorImageFormat = vk::Format::eR16G16B16A16Sfloat;
    VmaAllocation colorImageAllocation;
    vk::Image colorImage;
    vk::ImageView colorImageView;
    vk::DescriptorImageInfo colorDescriptorImageInfo;

    vk::RenderPass renderPass;

    vk::CommandBuffer commandBuffer;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;
    vk::Fence renderFinishedFence;

    std::unique_ptr<PipelineImage::PipelineImage> imagePipeline;
    std::unique_ptr<PipelineFlat::PipelineFlat> flatPipeline;
    std::unique_ptr<PipelineBox::PipelineBox> boxPipeline;
    std::unique_ptr<PipelineSDF::PipelineSDF> SDFPipeline;
    std::unique_ptr<PipelineToneMapper::PipelineToneMapper> toneMapperPipeline;

    gui_window_vulkan(gui_system &system, std::weak_ptr<gui_window_delegate> const &delegate, label const &title);
    ~gui_window_vulkan();

    gui_window_vulkan(const gui_window_vulkan &) = delete;
    gui_window_vulkan &operator=(const gui_window_vulkan &) = delete;
    gui_window_vulkan(gui_window_vulkan &&) = delete;
    gui_window_vulkan &operator=(gui_window_vulkan &&) = delete;

    void init() override;

    gui_device_vulkan &vulkan_device() const noexcept;

    void render(hires_utc_clock::time_point displayTimePoint) override;

    /*! Query the surface from the operating-system window.
     */
    virtual vk::SurfaceKHR getSurface() const = 0;

protected:
    void teardown() override;
    void build() override;
    
private:
    std::optional<uint32_t> acquireNextImageFromSwapchain();
    void presentImageToQueue(uint32_t frameBufferIndex, vk::Semaphore renderFinishedSemaphore);

    void fillCommandBuffer(vk::Framebuffer frameBuffer, aarect scissor_rectangle);
    void submitCommandBuffer();

    bool readSurfaceExtent();
    bool checkSurfaceExtent();

    void buildDevice();
    void buildSemaphores();
    void teardownSemaphores();
    gui_window_state buildSwapchain();
    void teardownSwapchain();
    void buildCommandBuffers();
    void teardownCommandBuffers();
    void buildRenderPasses();
    void teardownRenderPasses();
    void buildFramebuffers();
    void teardownFramebuffers();
    void buildPipelines();
    void teardownPipelines();
    bool buildSurface();
    void teardownSurface();
    void teardownDevice();

    void waitIdle();
    std::tuple<uint32_t, vk::Extent2D> getImageCountAndExtent();
};

}
