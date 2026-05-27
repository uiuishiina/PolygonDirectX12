#pragma once
#include"Device.h"
#include<optional>

//@brief    /=== フェンスクラス ===/
class Fence final
{
public:
    //----------------------------------------------------------------------------------------------------

    //@brief    === フェンス作成関数 ===
    //@return   フェンス作成の成否
    [[nodiscard]] bool create_fence();

    //@brief    === シグナル取得関数 ===
    //@param    command_queue   コマンドキュー
    //@return   設定したシグナル値
    [[nodiscard]] std::optional<UINT64> signal(ID3D12CommandQueue* command_queue) noexcept;

    //@brief    === 終了済みシグナル値取得関数 ===
    //@return   直近終了済みのシグナル値
    [[nodiscard]] UINT64 get_completed_value() const noexcept;

    //@brief    === フェンス取得関数 ===
    //@return   フェンスポインター
    [[nodiscard]] ID3D12Fence* get_fence() const noexcept;

    //@brief    === 待機関数 ===
    //@param    fence_value 待機したい値
    void wait_event(UINT64 fence_value) const noexcept;

    //----------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------

    Microsoft::WRL::ComPtr<ID3D12Fence> fence_;         //フェンス
    HANDLE                              wait_event_{};  //イベントハンドル
    UINT64                              fence_value_{}; //シグナル値

    //----------------------------------------------------------------------------------------------------
};