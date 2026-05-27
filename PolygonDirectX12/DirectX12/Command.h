#pragma once
#include"Device.h"
#include<vector>

class GraphicsCommand final 
{
public:
	//----------------------------------------------------------------------------------------------------

	//@brief	=== コマンドオブジェクト作成クラス ===
	//@brief	type    コマンドタイプ
	//@brief	frame_buffer_size    描画サイクルのバッファサイズ ...デフォルトで 2 指定
	//@return	コマンドオブジェクト作成の可否
	[[nodiscard]] bool create_command(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size = 2);

	//@brief    === コマンドキュー取得関数 ===
	//@return    コマンドキューポインター
	[[nodiscard]] ID3D12CommandQueue* get_queue()const noexcept;

	//@brief    === コマンドアロケーター取得関数 ===
	//@param    index    取得したいインデックス番号
	//@return    コマンドアロケーターポインター
	[[nodiscard]] ID3D12CommandAllocator* get_allocator(size_t index)const noexcept;

    //@brief    === コマンドリスト取得関数 ===
    //@return    コマンドリストポインター
    [[nodiscard]] ID3D12GraphicsCommandList* get_list()const noexcept;

	//@brief    === コマンドアロケーターリセット関数 ===
	//@param    リセットするコマンドアロケーターのインデックス番号
	void reset_allocator(size_t index);

	//@brief    === コマンドリストリセット関数 ===
	//@param    allocator    タイプ一致したコマンドアロケーター
	void reset_list(ID3D12CommandAllocator* allocator);

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue_{};    //Queue
	std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> allocator_;    //コマンドアロケーター配列
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list_{};    //コマンドリスト

	//----------------------------------------------------------------------------------------------------

	//@brief    === コマンドキュー初期化関数 ===
	//@return    キューの作成の成否
	[[nodiscard]] bool initialize_queue(D3D12_COMMAND_LIST_TYPE type);

	//@brief    === コマンドアロケーター初期化関数 ===
	//@param    type    コマンドタイプ
	//@param    frame_buffer_size    描画サイクルのバッファサイズ
	//@return    アロケーター作成の可否
	[[nodiscard]] bool initialize_allocator(D3D12_COMMAND_LIST_TYPE type, int frame_buffer_size);

	//@brief    === コマンドリスト初期化関数 ===
	//@param    alocator    タイプ一致したコマンドアロケーター
	//@param    type    コマンドタイプ
	//@return    リスト作成の可否
	[[nodiscard]] bool initialize_list(D3D12_COMMAND_LIST_TYPE type);

	//----------------------------------------------------------------------------------------------------
};