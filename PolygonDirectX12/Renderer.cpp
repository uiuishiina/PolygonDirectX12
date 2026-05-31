#include"DirectX12/Device.h"
#include"DirectX12/Command.h"
#include"DirectX12/SwapChain.h"
#include"DirectX12/RenderTarget.h"
#include"DirectX12/Fence.h"

#include"DirectX12/DescriptorHeap.h"

#include"DirectX12/ShaderCompiler.h"
#include"DirectX12/PolygonBase.h"
#include"DirectX12/RootSignature.h"
#include"DirectX12/PiplineState.h"
#include"DirectX12/ColorBuffer.h"

#include"IMGUI/ImguiManager.h"
#include"IMGUI/ColorEditer.h"

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

    hwnd_ = hwnd;

    //Device DXGI作成
    Check_Failed(Device::Instance().initialize_Device());

    //Command関係作成
    graphics_command = std::make_unique<GraphicsCommand>();
    Check_Failed(graphics_command->create_command(Direct_type, frame_buffer_size));

    //SwapChain作成
    swap_chain = std::make_unique<SwapChain>();
    Check_Failed(swap_chain->create_swapchain(graphics_command->get_queue(), hwnd_, 1280, 720, frame_buffer_size));

    //RenderTarget作成
    render_target = std::make_unique<RenderTarget>();
    Check_Failed(render_target->create_render_target(swap_chain->get_swapchain(), frame_buffer_size));

    //Fence作成
    fence_ = std::make_unique<Fence>();
    Check_Failed(fence_->create_fence());

    frame_fence_value.resize(frame_buffer_size,0);

    vs_shader_ = std::make_unique<ShaderCompiler>();
    ps_shader_ = std::make_unique<ShaderCompiler>();
    Check_Failed(vs_shader_->compile_shader(L"../DirectX12/Shader/VertexShader.hlsl", "main", "vs_5_0"));
    Check_Failed(ps_shader_->compile_shader(L"../DirectX12/Shader/PixelShader.hlsl", "main", "ps_5_0"));

    polygon_ = std::make_unique<PolygonBase>();
    VertexData vertex_date{};
    vertex_date.vertex_vec = {
        Vertex({0,0.5,0}),
        Vertex({0.5,-0.5,0}),
        Vertex({-0.5,-0.5,0})
    }; 
    vertex_date.index_vec = { 0,1,2 };

    Check_Failed(polygon_->create_polygon(vertex_date));


    heap_ = std::make_unique<DescriptorHeap>();
    if (!heap_->create_heap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1, true)) {
        return false;
    }

    color_ = std::make_unique<ColorBuffer>();

    color_date = std::make_unique<colorDate>();
    color_date->color_date = {
        color(1,0,0,1),
        color(0,1,0,1),
        color(0,0,1,1)
    };
    Check_Failed(color_->create_color_buffer(*color_date, heap_->get_CPU_handle(0)));

    color_->map(*color_date);

    root_ = std::make_unique<RootSignature>();
    Check_Failed(root_->create_root_signature());

    pipline_ = std::make_unique<PiplineState>();
    Check_Failed(pipline_->create_pipline_state(root_->get_root_signature(), vs_shader_->get_shader_blob(), ps_shader_->get_shader_blob()));


    //===========================================================================
    //IMGUI作成 
     
    ImGuiManager::Instance().initialize(hwnd_, Device::Instance().get_device(),
        graphics_command->get_queue(), frame_buffer_size, DXGI_FORMAT_R8G8B8A8_UNORM);
    
    //===========================================================================


	return true;
}

//----------------------------------------------------------------------------------------------------

void Renderer::update_renderer() {

    const auto back_buffer_index = swap_chain->get_swapchain()->GetCurrentBackBufferIndex();

    ImGuiManager::Instance().new_frame();
#if defined(_DEBUG)
    ColorEditor::draw(*color_date);

    ImGuiIO& io = ImGui::GetIO();
    RECT rc;
    GetClientRect(hwnd_, &rc);

    io.DisplaySize = ImVec2(
        (float)(rc.right - rc.left),
        (float)(rc.bottom - rc.top)
    );
#endif

    if (frame_fence_value[back_buffer_index] != 0) {
        fence_->wait_event(frame_fence_value[back_buffer_index]);
    }

    graphics_command->reset_allocator(back_buffer_index);

    graphics_command->reset_list(graphics_command->get_allocator(back_buffer_index));

    auto pToRT = resource_Barrier(render_target->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    graphics_command->get_list()->ResourceBarrier(1, &pToRT);

    D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { render_target->get_rtv_handle(back_buffer_index) };
    graphics_command->get_list()->OMSetRenderTargets(1, handles, false, nullptr);

    const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // 黒でクリア
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

    graphics_command->get_list()->SetPipelineState(pipline_->get_pipline_state());
    graphics_command->get_list()->SetGraphicsRootSignature(root_->get_root_signature());

    color_->map(*color_date);

    ID3D12DescriptorHeap* heap[] = { heap_->get_heap() };
    graphics_command->get_list()->SetDescriptorHeaps(1, heap);
    graphics_command->get_list()->SetGraphicsRootDescriptorTable(0, heap_->get_GPU_handle(0));

    polygon_->draw_polygon(graphics_command->get_list());

    //--------------------------------------------------------------------------------------------------

    //===========================================================================

    // ImGui の描画先レンダーターゲットを設定
    D3D12_CPU_DESCRIPTOR_HANDLE rtHandle = render_target->get_rtv_handle(back_buffer_index);
    graphics_command->get_list()->OMSetRenderTargets(1, &rtHandle, false, nullptr);

    // ImGui の描画コマンドを積む（内部でヒープ切り替えも行う）
    ImGuiManager::Instance().render(graphics_command->get_list());

    //===========================================================================

    auto rtToP = resource_Barrier(render_target->get_render_target(back_buffer_index), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    graphics_command->get_list()->ResourceBarrier(1, &rtToP);

    graphics_command->get_list()->Close();

    ID3D12CommandList* ppCommandLists[] = { graphics_command->get_list() };
    graphics_command->get_queue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    swap_chain->get_swapchain()->Present(1, 0);

    const auto nextFenceValue = fence_->signal(graphics_command->get_queue());
    frame_fence_value[back_buffer_index] = nextFenceValue.value();
};

void Renderer::end_renderer() {
    for (auto& p : frame_fence_value) {
        fence_->wait_event(p);
    }
    ImGuiManager::Instance().shutdown();
}