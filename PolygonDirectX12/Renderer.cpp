#include"DirectX12/Device.h"
#include"DirectX12/Command.h"
#include"DirectX12/SwapChain.h"
#include"DirectX12/RenderTarget.h"
#include"DirectX12/Fence.h"

#include"Renderer.h"

//----------------------------------------------------------------------------------------------------

namespace {
    //@brief    === 関数失敗チェックマクロ ===
#define Check_Failed(expr)\
    do {\
        if(!(expr)) {\
            return false;\
        }\
    } while (0)

    //@brief    直接実行型
    D3D12_COMMAND_LIST_TYPE Direct_type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    //@brief    === リソースバリア変更関数 ===
    [[nodiscard]] D3D12_RESOURCE_BARRIER resource_Barrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
        D3D12_RESOURCE_BARRIER barrier{};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = resource;
        barrier.Transition.StateBefore = from;
        barrier.Transition.StateAfter = to;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        return barrier;
    }
};

//----------------------------------------------------------------------------------------------------

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

[[nodiscard]] bool Renderer::initialize_renderer(HWND hwnd) {

    //Device DXGI作成
    Check_Failed(Device::Instance().initialize_Device());

    //Command関係作成
    graphics_command = std::make_unique<GraphicsCommand>();
    Check_Failed(graphics_command->create_command(Direct_type, frame_buffer_size));

    //SwapChain作成
    swap_chain = std::make_unique<SwapChain>();
    Check_Failed(swap_chain->create_swapchain(graphics_command->get_queue(), hwnd, 1280, 720, frame_buffer_size));

    //RenderTarget作成
    render_target = std::make_unique<RenderTarget>();
    Check_Failed(render_target->create_render_target(swap_chain->get_swapchain(), frame_buffer_size));

    //Fence作成
    fence_ = std::make_unique<Fence>();
    Check_Failed(fence_->create_fence());

    frame_fence_value.resize(frame_buffer_size,0);

	return true;
}

//----------------------------------------------------------------------------------------------------

void Renderer::update_renderer() {

    const auto back_buffer_index = swap_chain->get_swapchain()->GetCurrentBackBufferIndex();

    if (frame_fence_value[back_buffer_index] != 0) {
        fence_->wait_event(frame_fence_value[back_buffer_index]);
    }

    graphics_command->reset_allocator(back_buffer_index);

    graphics_command->reset_list(graphics_command->get_allocator(back_buffer_index));

    auto pToRT = resource_Barrier(render_target->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    graphics_command->get_list()->ResourceBarrier(1, &pToRT);

    D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render_target->get_rtv_handle(back_buffer_index) };
    graphics_command->get_list()->OMSetRenderTargets(1, handles, false, nullptr);

    const float clearColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // 黒でクリア
    graphics_command->get_list()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

    // ビューポート設定
    D3D12_VIEWPORT viewport{};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(1280);
    viewport.Height = static_cast<float>(720);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    graphics_command->get_list()->RSSetViewports(1, &viewport);

    // シザー矩形設定
    D3D12_RECT scissorRect{};
    scissorRect.left = 0;
    scissorRect.top = 0;
    scissorRect.right = 1280;
    scissorRect.bottom = 720;
    graphics_command->get_list()->RSSetScissorRects(1, &scissorRect);

    //--------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------

    auto rtToP = resource_Barrier(render_target->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    graphics_command->get_list()->ResourceBarrier(1, &rtToP);

    graphics_command->get_list()->Close();

    ID3D12CommandList* ppCommandLists[] = { graphics_command->get_list() };
    graphics_command->get_queue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    swap_chain->get_swapchain()->Present(1, 0);

    const auto nextFenceValue = fence_->signal(graphics_command->get_queue());
    frame_fence_value[back_buffer_index] = nextFenceValue.value();
}