#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl/client.h>
#include"../addfiles/d3dx12.h"

class Device final
{
public:
    //----------------------------------------------------------------------------------------------------

    //@brief    === Deviceインスタンス取得関数 ===
    //@return   シングルトンインスタンス
    static Device& Instance()noexcept {
        static Device instance_;
        return instance_;
    }

    //@brief    === Device初期化関数 ===
    //@return    初期化の可否
    [[nodiscard]] bool initialize_Device();

    //@brief    ===  DXGIFactory取得関数  ===
    //@return    DXGIFactoryポインター
    [[nodiscard]] IDXGIFactory6* get_factory()const noexcept;

    //@brief    ===  DXGIAdaptor取得関数  ===
    //@return    DXGIAdaptorポインター
    [[nodiscard]] IDXGIAdapter4* get_adaptor()const noexcept;

    //@brief    === Device取得関数 ===
    //@return    ID3D12Deviceポインター
    [[nodiscard]] ID3D12Device* get_device()const noexcept;

    //----------------------------------------------------------------------------------------------------
private:
    //----------------------------------------------------------------------------------------------------
    Device() = default;
    ~Device() = default;

	Microsoft::WRL::ComPtr<IDXGIFactory6> factory_{};    //DXGIFactory
	Microsoft::WRL::ComPtr<IDXGIAdapter4> adaptor_{};    //DXGIAdaptor
    Microsoft::WRL::ComPtr<ID3D12Device>    device_;    //Device

    //----------------------------------------------------------------------------------------------------

    //@brief    === DXGIFactory作成関数 ===
    //@return    作成の可否
    [[nodiscard]] bool create_factory();

    //@brief    ===  DXGIAdaptor選定関数  ===
    //@return    選定の可否
    [[nodiscard]] bool pick_adaptor();

    //@brief    === DXGIAdaptor条件探索関数 ===
    //@brief    条件に合うAdaptor
    [[nodiscard]] Microsoft::WRL::ComPtr<IDXGIAdapter4> find_adaptor(DXGI_GPU_PREFERENCE preference);

    //@brief    === Device作成関数 ===
    //@return    作成の可否
    [[nodiscard]] bool create_device();

    //@brief    === Device選定関数 ===
    //@return    作成したデバイス
    [[nodiscard]] Microsoft::WRL::ComPtr<ID3D12Device> select_device_by_level(IDXGIAdapter4* adapter, D3D_FEATURE_LEVEL featureLevel);

    //----------------------------------------------------------------------------------------------------
};