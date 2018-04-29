#include <Windows.h>
#include "OverlappedWindow.h"
#include <cassert>
#include <stdio.h>

COverlappedWindow::COverlappedWindow() {
	handle = 0;
	timerId = 0;
	time = 0;
	ellipse.pos.x = 300;
	ellipse.pos.y = 300;
	ellipse.radius = 30;
	ellipse.color = CreateSolidBrush(RGB(0, 150, 0));
	backgroundColor = (HBRUSH)3;
}

COverlappedWindow::~COverlappedWindow() {
	DeleteObject(ellipse.color);
	DeleteObject(backgroundColor);
}

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
		L"OverlappedWindow", L"Window", WS_OVERLAPPEDWINDOW, 0, 0,
		800, 600, 0, 0, GetModuleHandle(0), this);
	return (handle != 0);
}


void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnDestroy() {
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void COverlappedWindow::OnTimer() {
	UpdateCoordinates();
	InvalidateRect(handle, NULL, FALSE);
} 

void COverlappedWindow::UpdateCoordinates() {
	RECT rect;
	::GetClientRect(handle, &rect);
	if ((ellipse.pos.x + ellipse.speedX > rect.right - ellipse.radius) || (ellipse.pos.x + ellipse.speedX < rect.left + ellipse.radius)) {
		ellipse.speedX *= -1;
	}
	if ((ellipse.pos.y + ellipse.speedY > rect.bottom - ellipse.radius) || (ellipse.pos.y + ellipse.speedY < rect.top + ellipse.radius)) {
		ellipse.speedY *= -1;
	}
	ellipse.pos.x += ellipse.speedX;
	ellipse.pos.y += ellipse.speedY;
}

void COverlappedWindow::OnPaint() {
	PAINTSTRUCT ps;
	RECT rect;

	HDC dc = ::BeginPaint(handle, &ps);
	GetClientRect(handle, &rect);
	HDC bufferDC = CreateCompatibleDC(dc);
	HBITMAP buffer = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBuffer = (HBITMAP)SelectObject(bufferDC, buffer);
	FillRect(bufferDC, &rect, backgroundColor);

	SelectObject(bufferDC, ellipse.color);
	Ellipse(bufferDC, ellipse.pos.x - ellipse.radius, ellipse.pos.y - ellipse.radius,
			ellipse.pos.x + ellipse.radius, ellipse.pos.y + ellipse.radius);

	BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, oldBuffer);
	DeleteObject(buffer);
	DeleteDC(bufferDC);
	::EndPaint(handle, &ps);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->timerId = ::SetTimer(handle, 0, 25, 0);
		return 0;
	}
	case WM_DESTROY:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		COverlappedWindow* window = (COverlappedWindow*)(ULongToPtr(GetWindowLongPtr(handle, GWLP_USERDATA)));
		window->OnPaint();
		return 0;
	}
	case WM_TIMER:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnTimer();
		return 0;
	}

	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
