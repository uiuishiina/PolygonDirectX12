#include"Command.h"

//----------------------------------------------------------------------------------------------------

//@brief	=== コマンドオブジェクト作成クラス ===
//@brief	type    コマンドタイプ
//@brief	frame_buffer_size    描画サイクルのバッファサイズ ...デフォルトで 2 指定
//@return	コマンドオブジェクト作成の可否
[[nodiscard]] bool GraphicsCommand::create_command(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size) {

    if (!initialize_queue(type)) {
        return false;
    }
    if (!initialize_allocator(type, frame_buffer_size)) {
        return false;
    }
    if (!initialize_list(type)) {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === コマンドキュー初期化関数 ===
//@return    キューの作成の成否
[[nodiscard]] bool GraphicsCommand::initialize_queue(D3D12_COMMAND_LIST_TYPE type) {
    //コマンドキュー設定
    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.Type = type;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    //コマンドキュー作成
    const auto hr = Device::Instance().get_device()->CreateCommandQueue(&desc, IID_PPV_ARGS(&queue_));
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

//@brief    === コマンドアロケーター初期化関数 ===
//@param    type    コマンドタイプ
//@param    frame_buffer_size    描画サイクルのバッファサイズ
//@return    アロケーター作成の可否
[[nodiscard]] bool GraphicsCommand::initialize_allocator(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size) {
    //アロケーター配列初期化
    allocator_.resize(frame_buffer_size);

    //順に作成
    for (int i = 0; i < frame_buffer_size; i++) {
        const auto hr = Device::Instance().get_device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator_[i]));
        if (FAILED(hr)) {
            return false;
        }
    }
    return true;
}

//@brief    === コマンドリスト初期化関数 ===
//@param    alocator    タイプ一致したコマンドアロケーター
//@param    type    コマンドタイプ
//@return    リスト作成の可否
[[nodiscard]] bool GraphicsCommand::initialize_list(D3D12_COMMAND_LIST_TYPE type) {
    
    //コマンドリスト作成
    const auto hr = Device::Instance().get_device()->CreateCommandList(0, type, allocator_[0].Get(), nullptr, IID_PPV_ARGS(&list_));
    if (FAILED(hr)) {
        return false;
    }

    //コマンドリストを閉じる
    list_->Close();
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === コマンドキュー取得関数 ===
//@return    コマンドキューポインター
[[nodiscard]] ID3D12CommandQueue* GraphicsCommand::get_queue()const noexcept {
    return queue_.Get();
}

//@brief    === コマンドアロケーター取得関数 ===
//@param    index    取得したいインデックス番号
//@return    コマンドアロケーターポインター
[[nodiscard]] ID3D12CommandAllocator* GraphicsCommand::get_allocator(size_t index)const noexcept {
    return allocator_[index].Get();
}

//@brief    === コマンドリスト取得関数 ===
//@return    コマンドリストポインター
[[nodiscard]] ID3D12GraphicsCommandList* GraphicsCommand::get_list()const noexcept {
    return list_.Get();
}

//@brief    === コマンドアロケーターリセット関数 ===
//@param    リセットするコマンドアロケーターのインデックス番号
void GraphicsCommand::reset_allocator(size_t index) {
    allocator_[index]->Reset();
}

//@brief    === コマンドリストリセット関数 ===
//@param    allocator    タイプ一致したコマンドアロケーター
void GraphicsCommand::reset_list(ID3D12CommandAllocator* allocator) {
    list_->Reset(allocator, nullptr);
}