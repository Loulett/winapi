#include "BorderField.h"
#include <StrSafe.h>

bool BorderField::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = BorderField::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"BorderField";
	return (::RegisterClassEx(&ws) != 0);
}

bool BorderField::Create(HWND parentHandle) {
	handle = CreateWindowEx(0,
		L"BorderField", L"Child Window", WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), this);
	return (handle != 0);
}

void BorderField::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void BorderField::OnPaint() {
	PAINTSTRUCT ps;
	RECT rect;

	HDC dc = ::BeginPaint(handle, &ps);
	GetClientRect(handle, &rect);
	HDC bufferDC = CreateCompatibleDC(dc);
	HBITMAP buffer = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBuffer = (HBITMAP)SelectObject(bufferDC, buffer);

	FillRect(bufferDC, &rect, CreateSolidBrush(RGB(255, 255, 255)));

	BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, oldBuffer);
	DeleteObject(buffer);
	DeleteDC(bufferDC);

	::EndPaint(handle, &ps);
}

LRESULT BorderField::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE: {
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_PAINT: {
		InvalidateRect(handle, NULL, FALSE);
		BorderField* border = (BorderField*)GetWindowLongPtr(handle, GWLP_USERDATA);
		border->OnPaint();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}