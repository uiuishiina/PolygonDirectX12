#include"window/window.h"
#include"Renderer.h"

#include"Application.h"

namespace {
//チェックマクロ
#define Check_Failed(ans)\
do {\
	if(!(ans)) {\
		return false;\
	}\
} while (0);

}
 
Application::Application() = default;
Application::~Application() = default;

[[nodiscard]] bool Application::initialize_App(HINSTANCE hInstance) {

	window_ = std::make_unique<window>();
	Check_Failed(window_->create_window(hInstance, 1280, 720));

	renderer_ = std::make_unique<Renderer>();
	Check_Failed(renderer_->initialize_renderer(window_->get_hwnd()));

	return true;
}

void Application::run_App() {
	while (message_loop()) {
		renderer_->update_renderer();
	}

	renderer_->end_renderer();
}

[[nodiscard]] bool Application::message_loop() {
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}