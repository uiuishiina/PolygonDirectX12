#include"DescriptorHeap.h"

//----------------------------------------------------------------------------------------------------

//@bruef	=== ディスクリプターヒープ作成関数 ===
//@param	type	ヒープタイプ
//@param	num_descriptors		ヒープサイズ
//@param	flags		シェーダー可視フラグ
//@return	ヒープ作成の可否
[[nodiscard]] bool DescriptorHeap::create_heap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT num_descriptors, bool flags) {

    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = num_descriptors;
    desc.Flags = flags ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    const auto hr = Device::Instance().get_device()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&heap_));
    if (FAILED(hr)) {
        return false;
    }

    descriptor_size_ = Device::Instance().get_device()->GetDescriptorHandleIncrementSize(type);
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== CPUハンドル取得関数 ===
//@param	index	インデックス番号
//@return	オフセット済みCPUハンドル
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::get_CPU_handle(UINT index) {

    auto handle = heap_->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += descriptor_size_ * index;
    return handle;
}

//@brief	=== GPUハンドル取得関数 ===
//@param	index	インデックス番号
//@return	オフセット済みGPUハンドル
[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::get_GPU_handle(UINT index) {
    auto handle = heap_->GetGPUDescriptorHandleForHeapStart();
    handle.ptr += descriptor_size_ * index;
    return handle;
}

//----------------------------------------------------------------------------------------------------

//@brief	=== ディスクリプターヒープ取得関数 ===
//@return	ディスクリプターヒープポインター
[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::get_heap() const noexcept {
    return heap_.Get();
}