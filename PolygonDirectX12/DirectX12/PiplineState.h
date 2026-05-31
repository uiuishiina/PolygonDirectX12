#pragma once
#include"Device.h"

using Microsoft::WRL::ComPtr;

class PiplineState final
{
public:
    
    [[nodiscard]] bool create_pipline_state(ID3D12RootSignature* root_signature, ID3DBlob* vs_shader, ID3DBlob* ps_shader);

    [[nodiscard]] ID3D12PipelineState* get_pipline_state() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipline_state_;
};