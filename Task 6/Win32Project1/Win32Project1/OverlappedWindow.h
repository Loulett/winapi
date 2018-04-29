#pragma once
#include <Windows.h>

class COverlappedWindow
{
public:
	COverlappedWindow();
	~COverlappedWindow();

	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);

protected:
	void OnDestroy();
	void OnPaint();
	void OnTimer();
	void UpdateCoordinates();

private:
	HWND handle;
	HDC dc;
	UINT_PTR timerId;
	int time = 0;
	HBRUSH backgroundColor;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);

	struct Circle {
		POINT pos;
		int radius;
		int speedX = 3, speedY = 3;
		HBRUSH color;
	} ellipse;
};
