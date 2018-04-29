#include <Windows.h>
#include <array>
#include "CEllipseWindow.h"

class COverlappedWindow {
public:
	static bool RegisterClass();
	bool Create();
	void Show(int cmdShow);

protected:
	void OnSize();
	void OnCreate();

private:
	HWND handle;
	std::array<CEllipseWindow, 4> children;
	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};