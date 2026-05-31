#include"PiplineState.h"


[[nodiscard]] bool PiplineState::create_pipline_state(ID3D12RootSignature* root_signature, ID3DBlob* vsshader, ID3DBlob* psshader) {

    const D3D12_INPUT_ELEMENT_DESC input_layout[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};

    desc.pRootSignature = root_signature;

    desc.InputLayout = { input_layout, _countof(input_layout) };
    desc.VS = { vsshader->GetBufferPointer(),vsshader->GetBufferSize() };
    desc.PS = { psshader->GetBufferPointer(),psshader->GetBufferSize() };
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

    CD3DX12_BLEND_DESC blendDesc(D3D12_DEFAULT);
    auto& rt = blendDesc.RenderTarget[0];
    rt.BlendEnable = TRUE;
    rt.LogicOpEnable = FALSE;

    rt.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    rt.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    rt.BlendOp = D3D12_BLEND_OP_ADD;

    rt.SrcBlendAlpha = D3D12_BLEND_ONE;
    rt.DestBlendAlpha = D3D12_BLEND_ZERO;
    rt.BlendOpAlpha = D3D12_BLEND_OP_ADD;

    rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    desc.BlendState = blendDesc;


    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    desc.DepthStencilState.DepthEnable = FALSE;
    desc.DepthStencilState.StencilEnable = FALSE;

    desc.SampleMask = UINT_MAX;

    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;

    desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;

    // desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

    HRESULT hr = Device::Instance().get_device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipline_state_));
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

[[nodiscard]] ID3D12PipelineState* PiplineState::get_pipline_state() const noexcept {
    return pipline_state_.Get();
}