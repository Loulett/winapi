#include <algorithm>
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
	CheckerBoard::RegisterClass();
	board.Create(handle);
	BorderField::RegisterClass();
	border1.Create(handle);
	border2.Create(handle);
	return (handle != 0);
}

void BaseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	board.Show(cmdShow);
	border1.Show(cmdShow);
	border2.Show(cmdShow);
}

void BaseWindow::OnCreate() {
}

void BaseWindow::OnSize() {
	RECT rect;
	::GetClientRect(handle, &rect);
	int width = rect.right - rect.left, height = rect.bottom - rect.top;
	int x, y;
	if (height < width) {
		x = rect.left + width / 2 - height / 2;
		y = rect.top;
		SetWindowPos(border1.GetHandle(), HWND_TOP, rect.left, rect.top, width / 2 - height / 2, height, 0);
		SetWindowPos(border2.GetHandle(), HWND_TOP, rect.left + width / 2 + height / 2, rect.top, width / 2 - height / 2, height, 0);
	}
	else {
		x = rect.left;
		y = rect.top + height / 2 - width / 2;
		SetWindowPos(border1.GetHandle(), HWND_TOP, rect.left, rect.top, width, height / 2 - width / 2, 0);
		SetWindowPos(border2.GetHandle(), HWND_TOP, rect.left, rect.top + width / 2 + height / 2, width, height / 2 - width / 2, 0);
	}
	SetWindowPos(board.GetHandle(), HWND_TOP, x, y, min(width, height), min(width, height), 0);
}

void BaseWindow::EndGame(int color) {
	int msgBoxID = MessageBox(NULL, (LPCWSTR)L"End of Game!", (LPCWSTR)L"Checkers", MB_OKCANCEL);

	switch (msgBoxID) {
	case IDOK: {
		DestroyWindow(handle);
		break;
	}
	case IDCANCEL: {
		break;
	}
	}
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
	case WM_SIZE:
	{
		BaseWindow* window = (BaseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
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