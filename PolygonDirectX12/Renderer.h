#pragma once
#include<memory>
#include<vector>

//----------------------------------------------------------------------------------------------------

struct HWND__;
using HWND = HWND__*;

class GraphicsCommand;
class SwapChain;
class RenderTarget;
class Fence;

class DescriptorHeap;
class PolygonBase;
class RootSignature;
class PiplineState;
class ShaderCompiler;
class ColorBuffer;

struct colorDate;
//----------------------------------------------------------------------------------------------------

class Renderer final
{
public:
	//----------------------------------------------------------------------------------------------------
	Renderer();
	~Renderer();


	[[nodiscard]] bool initialize_renderer(HWND hwnd);

	void update_renderer();

	void end_renderer();

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	HWND hwnd_{};
	const int frame_buffer_size = 2;
	std::vector<UINT64>    frame_fence_value{};

	std::unique_ptr<colorDate> color_date{};

	std::unique_ptr<GraphicsCommand> graphics_command{};
	std::unique_ptr<SwapChain> swap_chain{};
	std::unique_ptr<RenderTarget> render_target{};
	std::unique_ptr<Fence> fence_{};

	std::unique_ptr<ShaderCompiler> vs_shader_{};
	std::unique_ptr<ShaderCompiler> ps_shader_{};

	std::unique_ptr<DescriptorHeap> heap_{};

	std::unique_ptr<PolygonBase> polygon_{};
	std::unique_ptr<ColorBuffer> color_{};
	std::unique_ptr<RootSignature> root_{};
	std::unique_ptr<PiplineState> pipline_{};

	//----------------------------------------------------------------------------------------------------
};