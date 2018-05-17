#pragma once
#include <Windows.h>
#include "CEditControl.h"

class COverlappedWindow {
public:
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);

protected:
	void OnCreate();
	void OnSize();
	bool OnClose();
	void OnDestroy();
	void OnCommand(WPARAM wParam, LPARAM lParam);

private:
	HWND handle;
	HMENU menu;
	bool textChanged = false;
	CEditControl child;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
	static LRESULT CALLBACK dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void saveText();
};