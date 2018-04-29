#include "COverlappedWindow.h"

bool COverlappedWindow::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = COverlappedWindow::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"OverlappedWindow";
	return (::RegisterClassEx(&ws) != 0);
}

bool COverlappedWindow::Create() {
	handle = CreateWindowEx(0,
		L"OverlappedWindow", L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	CEllipseWindow::RegisterClass();
	for (CEllipseWindow& window : children) {
		window.Create(handle);
	}
	return (handle != 0);
}

void COverlappedWindow::OnCreate() {
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	for (CEllipseWindow& window : children) {
		window.Show(cmdShow);
	}
}

void COverlappedWindow::OnSize() {
	RECT rect;
	int x, y, width, height;
	::GetClientRect(handle, &rect);
	x = (rect.left + rect.right) / 2;
	y = (rect.top + rect.bottom) / 2;
	width = (rect.right - rect.left) / 2;
	height = (rect.bottom - rect.top) / 2;
	SetWindowPos(children[0].GetHandle(), HWND_TOP, rect.left, rect.top, width, height, 0);
	SetWindowPos(children[1].GetHandle(), HWND_TOP, x, rect.top, width, height, 0);
	SetWindowPos(children[2].GetHandle(), HWND_TOP, rect.left, y, width, height, 0);
	SetWindowPos(children[3].GetHandle(), HWND_TOP, x, y, width, height, 0);
}



LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);;
	}
	case WM_SIZE:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);;
	}
	case WM_DESTROY:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}