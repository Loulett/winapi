#pragma once
#include <Windows.h>
#include "CEditControl.h"
#include "Dialog.h"

class COverlappedWindow {
public:
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);

	BYTE GetOpacity() { return opacity; }
	void SetOpacity(BYTE newOpacity) { opacity = newOpacity;  }
	DWORD GetFontColor() { return fontColor; }
	void SetFontColor(DWORD newFontColor) { fontColor = newFontColor; }
	DWORD GetBgColor() { return bgColor; }
	void SetBgColor(DWORD newBgColor) { bgColor = newBgColor;}

protected:
	void OnCreate();
	void OnSize();
	bool OnClose();
	void OnDestroy();
	void OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnCtlColorEdit(HDC hdc);

private:
	HWND handle;
	HMENU menu;
	bool textChanged = false;
	CEditControl child;
	Dialog dialog;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
	void saveText();
	BYTE opacity;
	DWORD fontColor = RGB(0, 0, 0), bgColor = RGB(255, 255, 255);
	HBRUSH brush;
};