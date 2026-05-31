#include"Device.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

//----------------------------------------------------------------------------------------------------

namespace {
    //    要求するD3D機能レベルのリスト...DirectX 12.2からDirectX 11.0までの機能レベルを指定
    D3D_FEATURE_LEVEL levels[] = {
    D3D_FEATURE_LEVEL_12_2,    //    49664
    D3D_FEATURE_LEVEL_12_1,    //    49408
    D3D_FEATURE_LEVEL_12_0,    //    49152
    D3D_FEATURE_LEVEL_11_1,    //    45056
    D3D_FEATURE_LEVEL_11_0    //    40960
    };
    //    要求するGPUの優先度のリスト...高性能GPU、デフォルトGPU、最低消費電力GPUの順で指定    
    DXGI_GPU_PREFERENCE preferences[] = {
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,   //    2
        DXGI_GPU_PREFERENCE_UNSPECIFIED,        //    0
        DXGI_GPU_PREFERENCE_MINIMUM_POWER       //    1
    };
}

//----------------------------------------------------------------------------------------------------

//@brief    === Device初期化関数 ===
//@return    初期化の可否
[[nodiscard]] bool Device::initialize_Device() {

    if (!create_factory()) {
        return false;
    }
    if (!pick_adaptor()) {
        return false;
    }
    if (!create_device()) {
        return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------

//@brief    === DXGIFactory作成関数 ===
//@return    作成の可否
[[nodiscard]] bool Device::create_factory() {
#if _DEBUG
    ComPtr<ID3D12Debug> debug{};
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif
    UINT Flags = 0;
#if _DEBUG
    Flags |= DXGI_CREATE_FACTORY_DEBUG;    // デバッグフラグを設定
#endif
    const auto hr = CreateDXGIFactory2(Flags, IID_PPV_ARGS(&factory_));
    if (FAILED(hr)) {
        return false;
    }
    return true;
}

//@brief    ===  DXGIAdaptor選定関数  ===
//@return    選定の可否
[[nodiscard]] bool Device::pick_adaptor() {
    //    要求するGPUの優先度のリストを順に試して、最初に見つかった適切なGPUを選択
    for (const auto preference : preferences) {
        adaptor_ = find_adaptor(preference);
        if (adaptor_) {
            return true;
        }
    }
    return false;
}

//@brief    === DXGIAdaptor条件探索関数 ===
//@brief    条件に合うAdaptor
[[nodiscard]] Microsoft::WRL::ComPtr<IDXGIAdapter4> Device::find_adaptor(DXGI_GPU_PREFERENCE preference) {
    for (int i = 0; ; ++i) {
        DXGI_ADAPTER_DESC1 desc{};
        ComPtr<IDXGIAdapter4> dxgiAdapter{};

        // GPUの優先度設定してアダプタを列挙
        if (FAILED(factory_->EnumAdapterByGpuPreference(i, preference, IID_PPV_ARGS(&dxgiAdapter)))) {
            break;
        }

        dxgiAdapter->GetDesc1(&desc);
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            continue;
        }

        return dxgiAdapter;
    }
    return nullptr;
}

//@brief    === Device作成関数 ===
//@return    作成の可否
[[nodiscard]] bool Device::create_device() {

    //リストに定義してあるレベルを試す
    for (const auto& level : levels) {
        device_ = select_device_by_level(adaptor_.Get(), level);
        if (device_) {
            return true;
        }
    }
    return false;
}

//@brief    === Device選定関数 ===
//@return    作成したデバイス
[[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Device> Device::select_device_by_level(
    IDXGIAdapter4* adapter, D3D_FEATURE_LEVEL featureLevel) {
    
    ComPtr<ID3D12Device> device;
    const auto hr = D3D12CreateDevice(
        adapter,
        featureLevel,
        IID_PPV_ARGS(&device)
    );
    if (FAILED(hr)) {
        return nullptr;
    }
    return device;
}

//----------------------------------------------------------------------------------------------------

//@brief    ===  DXGIFactory取得関数  ===
//@return    DXGIFactoryポインター
[[nodiscard]] IDXGIFactory6* Device::get_factory()const noexcept {
    return factory_.Get();
}

//@brief    ===  DXGIAdaptor取得関数  ===
//@return    DXGIAdaptorポインター
[[nodiscard]] IDXGIAdapter4* Device::get_adaptor()const noexcept {
    return adaptor_.Get();
}

//@brief    === Device取得関数 ===
//@return    ID3D12Deviceポインター
[[nodiscard]] ID3D12Device* Device::get_device()const noexcept {
    return device_.Get();
}