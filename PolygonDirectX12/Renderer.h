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

//----------------------------------------------------------------------------------------------------

class Renderer final
{
public:
	//----------------------------------------------------------------------------------------------------
	Renderer();
	~Renderer();


	[[nodiscard]] bool initialize_renderer(HWND hwnd);

	void update_renderer();

	//----------------------------------------------------------------------------------------------------
private:
	//----------------------------------------------------------------------------------------------------

	const int frame_buffer_size = 2;
	std::vector<UINT64>    frame_fence_value{};

	std::unique_ptr<GraphicsCommand> graphics_command{};
	std::unique_ptr<SwapChain> swap_chain{};
	std::unique_ptr<RenderTarget> render_target{};
	std::unique_ptr<Fence> fence_{};

	//----------------------------------------------------------------------------------------------------
};