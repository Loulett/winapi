#pragma once
#include <Windows.h>

class CEditControl {
public:
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle();

private:
	HWND handle;
};