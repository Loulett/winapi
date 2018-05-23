#pragma once
#include <Windows.h>
#include <string>

class BorderField {
public:
	static bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle() { return handle; }

protected:
	void OnPaint();

private:
	HWND handle;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};