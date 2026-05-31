#pragma once
#include"Device.h"

class ShaderCompiler final
{
public:

    [[nodiscard]] bool compile_shader(const wchar_t* filename, const char* entry_point_name, const char* target_profile);

    [[nodiscard]] ID3DBlob* get_shader_blob() const noexcept;

private:
    Microsoft::WRL::ComPtr<ID3DBlob> shader_blob_;
};