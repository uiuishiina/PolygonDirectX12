#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

#include"../addfiles/imgui/imgui.h"
#include"../addfiles/imgui/backends/imgui_impl_win32.h"
#include"../addfiles/imgui/backends/imgui_impl_dx12.h"


class ImGuiManager final {
public:
	//---------------------------------------------------------------------------------
	
	static ImGuiManager& Instance() noexcept {
		static ImGuiManager ins;
		return ins;
	}

public:
	//---------------------------------------------------------------------------------
	
	[[nodiscard]] bool initialize(HWND hwnd,ID3D12Device* device,ID3D12CommandQueue* commandQueue,UINT frame_buffer_size,
		DXGI_FORMAT rtvFormat) noexcept;

	//---------------------------------------------------------------------------------
	
	void new_frame() noexcept;

	//---------------------------------------------------------------------------------
	
	void render(ID3D12GraphicsCommandList* commandList) noexcept;

	//---------------------------------------------------------------------------------

	void shutdown() noexcept;

private:
	//---------------------------------------------------------------------------------

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srv_heap_{};
};