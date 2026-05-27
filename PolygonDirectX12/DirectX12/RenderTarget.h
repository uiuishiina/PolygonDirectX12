#pragma once
#include"DescriptorHeap.h"

class RenderTarget final
{
public:

    [[nodiscard]] bool create_render_target(IDXGISwapChain4* swapchain, const UINT& buffer_size);

    [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE get_rtv_handle(const size_t& index) const noexcept;

    [[nodiscard]] ID3D12Resource* get_render_target(const size_t& index) const noexcept;

    void render_target_reset();

private:
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> render_targets_;
    std::unique_ptr<DescriptorHeap> rtv_heap{};
};