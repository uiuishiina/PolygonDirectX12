#include"SwapChain.h"

[[nodiscard]] bool SwapChain::create_swapchain(ID3D12CommandQueue* command_queue, HWND hwnd, const UINT& width, const UINT& height, const size_t& buffer_size) {

    DXGI_SWAP_CHAIN_DESC1 swapchain_desc{};
    swapchain_desc.Width = width;
    swapchain_desc.Height = height;
    swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.Stereo = FALSE;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SampleDesc.Quality = 0;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount = buffer_size;
    swapchain_desc.Scaling = DXGI_SCALING_STRETCH;
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchain_desc.Flags = 0;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapchain1{};
    auto hr = Device::Instance().get_factory()->CreateSwapChainForHwnd(command_queue, hwnd, &swapchain_desc, nullptr, nullptr, &swapchain1);
    if (FAILED(hr)) {
        return false;
    }

    hr = swapchain1.As(&swapchain_);
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

[[nodiscard]] IDXGISwapChain4* SwapChain::get_swapchain() const noexcept {
    return swapchain_.Get();
}