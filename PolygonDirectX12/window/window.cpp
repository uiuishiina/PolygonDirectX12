#include"window.h"


#include"../addfiles/imgui/backends/imgui_impl_win32.h"

// ImGui の Win32 メッセージハンドラ（imgui_impl_win32.h 内で宣言済み）
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace {
	static LRESULT CALLBACK static_window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        // ImGui にメッセージを転送する
        // ImGui がそのメッセージを消費した場合（戻り値が非ゼロ）はゲーム側に渡さない
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
            return true;
        }
		switch (msg)
		{
        case WM_NCCREATE:
            SetProcessDpiAwarenessContext(
                DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
            );
            break;
        case WM_KEYDOWN: {
            bool repeat = (lParam & (1 << 30)) != 0;
            if (repeat) return 0;
            switch (wParam) {
            case VK_ESCAPE:
                DestroyWindow(hwnd);
                break;
            case VK_LEFT:
                break;
            case VK_RIGHT:
                break;
            }
            return 0;
        }
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_NCDESTROY:
            PostQuitMessage(0);
			break;
		default:
			break;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

[[nodiscard]] bool window::create_window(HINSTANCE hInstance, int width, int height) {

    const wchar_t CLASS_NAME[] = L"MyDX12WindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = static_window_proc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClassW(&wc);

    hwnd_ = CreateWindowExW(
        0,
        CLASS_NAME,
        L"",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        NULL,
        hInstance,
        nullptr
    );

    ShowWindow(hwnd_, SW_SHOWDEFAULT);
    UpdateWindow(hwnd_);

    return true;
}

[[nodiscard]] HWND window::get_hwnd()const noexcept {
	return hwnd_;
}