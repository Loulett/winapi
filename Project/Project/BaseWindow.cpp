#include "BaseWindow.h"

bool BaseWindow::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = BaseWindow::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"BaseWindow";
	return (::RegisterClassEx(&ws) != 0);
}

bool BaseWindow::Create() {
	handle = CreateWindowEx(0,
		L"BaseWindow", L"Checkers", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void BaseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void BaseWindow::OnCreate() {
}

LRESULT BaseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}