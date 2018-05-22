#pragma once
#include <Windows.h>

struct Settings {
	DWORD fontColor, backGroundColor;
	LOGFONT font;
	BYTE opacity;
};

class Dialog {
public:
	HWND GetHandle();
	void Create(HINSTANCE hInstance, HWND parentHandle, HWND editControlHandle);

protected:
	void OnCreate(HWND handleDlg);
	void SelectColor(DWORD& targetColor);
	void OnScroll(LPARAM lParam);
	void OnCheck();
	void OnOk();
	void OnClose();

private:
	HWND handle, parentHandle, editControlHandle;
	bool preview = false;
	COLORREF palitre[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	Settings oldSettings, newSettings;

	static LRESULT CALLBACK dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam);
	void updateSettings(Settings& settings);
	void tryPreview();
};