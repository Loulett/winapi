#include <Windows.h>

class CEllipseWindow
{
public:
	CEllipseWindow();
	~CEllipseWindow();

	static bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();

protected:
	void OnDestroy();
	void OnPaint();
	void OnTimer();
	void OnLButtonDown();
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
