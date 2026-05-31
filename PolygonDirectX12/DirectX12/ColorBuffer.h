#pragma once
#include"Device.h"
#include<vector>

struct color {
    float R{}, G{}, B{}, A{};
};

struct colorDate {
    std::vector<color> color_date{};
};

class ColorBuffer final
{
public:
    [[nodiscard]] bool create_constant_Buffer(const colorDate& date, D3D12_CPU_DESCRIPTOR_HANDLE handle);

    [[nodiscard]] ID3D12Resource* get_buffer()const noexcept;

    void map(const colorDate& date);
private:
    Microsoft::WRL::ComPtr<ID3D12Resource>    color_buffer_;
    uint8_t* mapped_ = nullptr;
};