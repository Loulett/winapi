#pragma once
#include <Windows.h>
#include <array>

struct CheckerBoardField {
	HBRUSH color;
};

class CheckerBoard {
public:
	bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);

private:
	HWND handle;
	std::array<CheckerBoardField, 64> fields;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};