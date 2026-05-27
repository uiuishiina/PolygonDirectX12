#pragma once
#include"Device.h"

class DescriptorHeap final
{
public:
	//----------------------------------------------------------------------------------------------------

	//@bruef	=== ディスクリプターヒープ作成関数 ===
	//@param	type	ヒープタイプ
	//@param	num_descriptors		ヒープサイズ
	//@param	flags		シェーダー可視フラグ
	//@return	ヒープ作成の可否
	[[nodiscard]] bool create_heap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num_descriptors, bool flags);
	
	//@brief	=== CPUハンドル取得関数 ===
	//@param	index	インデックス番号
	//@return	オフセット済みCPUハンドル
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE get_CPU_handle(UINT index);

	//@brief	=== GPUハンドル取得関数 ===
	//@param	index	インデックス番号
	//@return	オフセット済みGPUハンドル
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE get_GPU_handle(UINT index);

	//@brief	=== ディスクリプターヒープ取得関数 ===
	//@return	ディスクリプターヒープポインター
	[[nodiscard]] ID3D12DescriptorHeap* get_heap() const noexcept;

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{};
	UINT descriptor_size_{};

	//----------------------------------------------------------------------------------------------------
};