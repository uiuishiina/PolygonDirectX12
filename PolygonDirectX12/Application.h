#pragma once
#include<memory>

struct HINSTANCE__;
using HINSTANCE = HINSTANCE__*;

class window;
class Renderer;

class Application final
{
public:
	Application();
	~Application();

	[[nodiscard]] bool initialize_App(HINSTANCE hInstance);

	void run_App();

private:
	
	std::unique_ptr<window> window_{};
	std::unique_ptr<Renderer>	renderer_{};

	[[nodiscard]] bool message_loop();
};