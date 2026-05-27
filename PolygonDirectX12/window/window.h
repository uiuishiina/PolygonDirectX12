#pragma once
#include<Windows.h>

class window final
{
public:
	[[nodiscard]] bool create_window(HINSTANCE hInstance, int width, int height);

	[[nodiscard]] HWND get_hwnd()const noexcept;
private:
	HWND hwnd_{};
};