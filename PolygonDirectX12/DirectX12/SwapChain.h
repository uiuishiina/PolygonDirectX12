#pragma once
#include"Device.h"

class SwapChain final
{
public:
    
    //@brief    === スワップチェーン作成関数 ===
    [[nodiscard]] bool create_swapchain(ID3D12CommandQueue* command_queue, HWND hwnd, const UINT& width, const UINT& height, const size_t& buffer_size);

    [[nodiscard]] IDXGISwapChain4* get_swapchain() const noexcept;

private:
    Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain_;
};