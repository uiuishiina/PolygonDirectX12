#pragma once
#include"Device.h"

class RootSignature final
{
public:

    [[nodiscard]] bool create_root_signature();

    [[nodiscard]] ID3D12RootSignature* get_root_signature()const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_{};
};