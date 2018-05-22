#include "CheckerBoard.h"

bool CheckerBoard::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = CheckerBoard::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"CheckerBoard";
	return (::RegisterClassEx(&ws) != 0);
}

bool CheckerBoard::Create(HWND parentHandle) {
	handle = CreateWindowEx(0,
		L"EllipseWindow", L"Child Window", WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void CheckerBoard::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

LRESULT CheckerBoard::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
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