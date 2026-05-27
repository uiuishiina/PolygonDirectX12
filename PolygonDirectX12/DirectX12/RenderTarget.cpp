#include"RenderTarget.h"

[[nodiscard]] bool RenderTarget::create_render_target(IDXGISwapChain4* swapchain, const UINT& buffer_size) {

    render_targets_.resize(static_cast<size_t>(buffer_size));

    rtv_heap = std::make_unique<DescriptorHeap>();

    if (!rtv_heap->create_heap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, buffer_size, false)) {
        return false;
    }

    for (UINT i = 0; i < buffer_size; ++i) {
        const auto hr = swapchain->GetBuffer(i, IID_PPV_ARGS(&render_targets_[i]));
        if (FAILED(hr)) {
            return false;
        }

        D3D12_RENDER_TARGET_VIEW_DESC rtv_desc{};
        rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        auto handle = rtv_heap->get_CPU_handle(i);
        Device::Instance().get_device()->CreateRenderTargetView(render_targets_[i].Get(), &rtv_desc, handle);
    }
    return true;
}

[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::get_rtv_handle(const size_t& index) const noexcept {
   return rtv_heap->get_CPU_handle(index);
}

[[nodiscard]] ID3D12Resource* RenderTarget::get_render_target(const size_t& index) const noexcept {
    return render_targets_[index].Get();
}

void RenderTarget::render_target_reset() {
    for (auto& p : render_targets_) {
        p.Reset();
    }
    render_targets_.clear();
}