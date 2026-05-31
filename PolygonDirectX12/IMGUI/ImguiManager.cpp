#include"ImguiManager.h"

//---------------------------------------------------------------------------------

bool ImGuiManager::initialize(HWND hwnd,ID3D12Device* device,ID3D12CommandQueue* commandQueue,UINT numFrames,
    DXGI_FORMAT rtvFormat) noexcept {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // スタイル設定（ダークテーマ）
    ImGui::StyleColorsDark();

    // ImGui 専用の CBV_SRV_UAV ヒープを作成する
    // DescriptorHeapContainer の共有ヒープとは独立させることで
    // 互いの管理コードが干渉しない
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NumDescriptors = 1;  // フォントテクスチャ 1 枚のみ
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    const HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&srv_heap_));
    if (FAILED(hr)) {
        return false;
    }

    // Win32 バックエンドの初期化
    if (!ImGui_ImplWin32_Init(hwnd)) {
        return false;
    }

    // DX12 バックエンドの初期化（ImGui 1.91.5+ の InitInfo 構造体 API を使用）
    // CommandQueue を渡すことで ImGuiBackendFlags_RendererHasTextures が有効になり
    // フォントテクスチャのアップロードがバックエンド内部で自動処理される
    ImGui_ImplDX12_InitInfo initInfo{};
    initInfo.Device = device;
    initInfo.CommandQueue = commandQueue;
    initInfo.NumFramesInFlight = static_cast<int>(numFrames);
    initInfo.RTVFormat = rtvFormat;
    initInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;
    initInfo.SrvDescriptorHeap = srv_heap_.Get();
    initInfo.LegacySingleSrvCpuDescriptor = srv_heap_->GetCPUDescriptorHandleForHeapStart();
    initInfo.LegacySingleSrvGpuDescriptor = srv_heap_->GetGPUDescriptorHandleForHeapStart();

    // 日本語フォントの読み込み
    ImGuiIO& io = ImGui::GetIO();
    const auto* font = io.Fonts->AddFontFromFileTTF(
        "C:\\Windows\\Fonts\\msgothic.ttc",  // MS ゴシックのフォントファイルパス
        14.0f,                               // フォントサイズ
        nullptr,                             // デフォルト設定
        io.Fonts->GetGlyphRangesJapanese()   // 日本語グリフ範囲
    );
    IM_ASSERT(font != nullptr);

    if (!ImGui_ImplDX12_Init(&initInfo)) {
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------------
/**
 * @brief	新しいフレームを開始する
 */
void ImGuiManager::new_frame() noexcept {
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // ウィンドウ : フレーム情報
    ImGui::Begin("フレーム情報");
    {
        // ImGui が内部で計算した平均 FPS を表示する
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("フレーム時間: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("ESCで終了");
    }
    ImGui::End();
}

//---------------------------------------------------------------------------------
/**
 * @brief	ImGui の描画コマンドをコマンドリストに積む
 */
void ImGuiManager::render(ID3D12GraphicsCommandList* commandList) noexcept {
    // ImGui の描画データを確定させる
    ImGui::Render();

    // ImGui 専用 SRV ヒープをコマンドリストにバインドする
    // ゲームの描画が全部終わった後に呼ぶので、共有ヒープへの影響はない
    ID3D12DescriptorHeap* heaps[] = { srv_heap_.Get() };
    commandList->SetDescriptorHeaps(1, heaps);

    // 実際の描画コマンドを積む
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

//---------------------------------------------------------------------------------
/**
 * @brief	ImGui をシャットダウンする
 */
void ImGuiManager::shutdown() noexcept {
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
