#pragma once
#include <Windows.h>

class BaseWindow
{
public:
	bool RegisterClass();
	bool Create();
	void Show(int cmdShow);

protected:
	void OnCreate();

private:
	HWND handle;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};