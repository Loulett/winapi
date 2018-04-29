#include <Windows.h>
#include "CEllipseWindow.h"
#include <cassert>
#include <stdio.h>

CEllipseWindow::CEllipseWindow() {
	handle = 0;
	timerId = 0;
	time = 0;
	ellipse.pos.x = -1;
	ellipse.pos.y = -1;
	ellipse.radius = 30;
	ellipse.color = CreateSolidBrush(RGB(0, 150, 0));
	backgroundColor = (HBRUSH)3;
}

CEllipseWindow::~CEllipseWindow() {
	DeleteObject(ellipse.color);
	DeleteObject(backgroundColor);
}

bool CEllipseWindow::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = CEllipseWindow::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"EllipseWindow";
	return (::RegisterClassEx(&ws) != 0);
}

bool CEllipseWindow::Create(HWND parentHandle) {
	handle = CreateWindowEx(0,
		L"EllipseWindow", L"Child Window", WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), this);
	return (handle != 0);
}


void CEllipseWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

HWND CEllipseWindow::GetHandle() {
	return handle;
}

void CEllipseWindow::OnDestroy() {
	KillTimer(handle, timerId);
	PostQuitMessage(0);
}

void CEllipseWindow::OnTimer() {
	if (::GetFocus() == handle) {
		UpdateCoordinates();
	}
	InvalidateRect(handle, NULL, FALSE);
}

void CEllipseWindow::OnLButtonDown() {
	SetFocus(handle);
}

void CEllipseWindow::UpdateCoordinates() {
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

void CEllipseWindow::OnPaint() {
	PAINTSTRUCT ps;
	RECT rect;

	HDC dc = ::BeginPaint(handle, &ps);
	GetClientRect(handle, &rect);
	HDC bufferDC = CreateCompatibleDC(dc);
	HBITMAP buffer = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBuffer = (HBITMAP)SelectObject(bufferDC, buffer);
	FillRect(bufferDC, &rect, backgroundColor);
	if (ellipse.pos.x == -1 && ellipse.pos.y == -1) {
		ellipse.pos.x = rect.left + ellipse.radius;
		ellipse.pos.y = rect.top + ellipse.radius;
	}

	if (::GetFocus() == handle) {
		ellipse.color = CreateSolidBrush(RGB(0, 250, 0));
	}
	else {
		ellipse.color = CreateSolidBrush(RGB(0, 150, 0));
	}

	SelectObject(bufferDC, ellipse.color);
	Ellipse(bufferDC, ellipse.pos.x - ellipse.radius, ellipse.pos.y - ellipse.radius,
		ellipse.pos.x + ellipse.radius, ellipse.pos.y + ellipse.radius);

	BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, oldBuffer);
	DeleteObject(buffer);
	DeleteDC(bufferDC);
	::EndPaint(handle, &ps);
}

LRESULT CEllipseWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->timerId = ::SetTimer(handle, 0, 25, 0);
		return 0;
	}
	case WM_DESTROY:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	case WM_PAINT:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnPaint();
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnLButtonDown();
		return 0;
	}
	case WM_TIMER:
	{
		CEllipseWindow* window = (CEllipseWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnTimer();
		return 0;
	}

	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}
