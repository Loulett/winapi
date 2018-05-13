#include "CEditControl.h"
#include <windows.h>
#include <strsafe.h>

HWND CEditControl::GetHandle() {
	return handle;
}

bool CEditControl::Create(HWND parentHandle) {
	handle = CreateWindowEx(0,
		L"EDIT", 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), 0);
	SetFocus(handle);
	return (handle != 0);
}

void CEditControl::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}