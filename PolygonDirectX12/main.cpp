#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include"Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int visible) {

	Application App;
	if (!App.initialize_App(hInstance)) {
		return -1;
	}
	App.run_App();

	return 0;
}