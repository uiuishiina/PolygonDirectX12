#include"Fence.h"

//----------------------------------------------------------------------------------------------------

//@brief    === フェンス作成関数 ===
//@return   フェンス作成の成否
[[nodiscard]] bool Fence::create_fence() {

    //フェンス作成
    const auto hr = Device::Instance().get_device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    if (FAILED(hr)) {
        return false;
    }

    //イベント作成
    wait_event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!wait_event_) {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === シグナル取得関数 ===
//@param    command_queue   コマンドキュー
//@return   設定したシグナル値
[[nodiscard]] std::optional<UINT64> Fence::signal(ID3D12CommandQueue* command_queue) noexcept {

    //シグナル値更新
    fence_value_++;

    //シグナル設定
    const auto hr = command_queue->Signal(fence_.Get(), fence_value_);
    if (FAILED(hr)) {
        return std::nullopt;
    }
    return fence_value_;
}

//----------------------------------------------------------------------------------------------------

//@brief    === 終了済みシグナル値取得関数 ===
//@return   直近終了済みのシグナル値
[[nodiscard]] UINT64 Fence::get_completed_value() const noexcept {
    return fence_->GetCompletedValue();
}

//----------------------------------------------------------------------------------------------------

//@brief    === フェンス取得関数 ===
//@return   フェンスポインター
[[nodiscard]] ID3D12Fence* Fence::get_fence() const noexcept {
    return fence_.Get();
}

//----------------------------------------------------------------------------------------------------

//@brief    === 待機関数 ===
//@param    fence_value 待機したい値
void Fence::wait_event(UINT64 fence_value) const noexcept {

    //待機したい値までスレッドを止める
    if (fence_->GetCompletedValue() < fence_value) {
        fence_->SetEventOnCompletion(fence_value, wait_event_);
        WaitForSingleObject(wait_event_, INFINITE);
    }
}