#pragma once
#include <Windows.h>
#include "CheckerBoard.h"
#include "BorderField.h"

class BaseWindow
{
public:
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);
	void EndGame(int color);

protected:
	void OnCreate();
	void OnSize();

private:
	HWND handle;
	CheckerBoard board;
	BorderField border1, border2;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};