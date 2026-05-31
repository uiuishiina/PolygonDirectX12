#include"ColorBuffer.h"

[[nodiscard]] bool ColorBuffer::create_color_buffer(const colorDate& date, D3D12_CPU_DESCRIPTOR_HANDLE handle) {

    if (date.color_date.empty()) {
        return false;
    }
    const auto size = (sizeof(color) * static_cast<UINT>(date.color_date.size()) + 255) & ~255;

    D3D12_HEAP_PROPERTIES heap{};
    heap.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    auto hr = Device::Instance().get_device()->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&color_buffer_));
    if (FAILED(hr)) {

        return false;
    }

    hr = color_buffer_->Map(0, nullptr, reinterpret_cast<void**>(&mapped_));
    if (FAILED(hr)) {
        return false;
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = (UINT)date.color_date.size();
    srvDesc.Buffer.StructureByteStride = sizeof(color);
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

    Device::Instance().get_device()->CreateShaderResourceView(color_buffer_.Get(), &srvDesc, handle);

    return true;
}

[[nodiscard]] ID3D12Resource* ColorBuffer::get_buffer()const noexcept {
    return color_buffer_.Get();
}

void ColorBuffer::map(const colorDate& date) {

    memcpy(mapped_,date.color_date.data(),sizeof(color) * date.color_date.size());
}