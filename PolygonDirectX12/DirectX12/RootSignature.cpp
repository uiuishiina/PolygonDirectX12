#include"RootSignature.h"


[[nodiscard]] bool RootSignature::create_root_signature() {
    
    CD3DX12_DESCRIPTOR_RANGE range;
    range.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER param;
    param.InitAsDescriptorTable(1, &range, D3D12_SHADER_VISIBILITY_VERTEX);

    CD3DX12_ROOT_SIGNATURE_DESC desc;
    desc.Init(1, &param, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    Microsoft::WRL::ComPtr<ID3DBlob> serialized;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob;
    auto hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &serialized, &error_blob);
    if (FAILED(hr)) {
        if (error_blob) {
        }
        return false;
    }

    hr = Device::Instance().get_device()->CreateRootSignature(0, serialized->GetBufferPointer(), serialized->GetBufferSize(), IID_PPV_ARGS(&root_signature_));
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

[[nodiscard]] ID3D12RootSignature* RootSignature::get_root_signature()const noexcept {
    return root_signature_.Get();
}