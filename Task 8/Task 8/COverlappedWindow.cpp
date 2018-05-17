#include "COverlappedWindow.h"
#include "resource.h"

bool COverlappedWindow::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = COverlappedWindow::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	ws.lpszClassName = L"OverlappedWindow";
	return (::RegisterClassEx(&ws) != 0);
}

bool COverlappedWindow::Create() {
	handle = CreateWindowEx(0,
		L"OverlappedWindow", L"Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, GetModuleHandle(0), this);
	child.Create(handle);
	menu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	return (handle != 0);
}

void COverlappedWindow::OnCreate() {
}

void COverlappedWindow::OnSize() {
	RECT rect;
	::GetClientRect(handle, &rect);
	SetWindowPos(child.GetHandle(), HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0);
}

bool COverlappedWindow::OnClose() {
	if (textChanged) {
		int msgBox = MessageBox(handle, L"Save Text?", L"Window", MB_YESNOCANCEL);
		switch (msgBox) {
		case IDYES:
			this->saveText();
			return true;
		case IDNO:
			return true;
		case IDCANCEL:
			return false;
		}
	}
	return true;
}

void COverlappedWindow::saveText() {
	size_t lengthMessage = SendMessage(child.GetHandle(), WM_GETTEXTLENGTH, 0, 0);
	wchar_t* text = new wchar_t[lengthMessage + 1];
	SendMessage(child.GetHandle(), WM_GETTEXT, lengthMessage + 1, (LPARAM)text);
	wchar_t fileName[100];
	OPENFILENAME opf = {};
	opf.lStructSize = sizeof(OPENFILENAME);
	opf.hwndOwner = handle;
	opf.lpstrFile = fileName;
	opf.lpstrFile[0] = '\0';
	opf.nMaxFile = sizeof(fileName);
	bool result = GetSaveFileName(&opf);
	if (result) {
		HANDLE fileHandle = CreateFile(fileName, GENERIC_WRITE, (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE), 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
		WriteFile(fileHandle, text, lengthMessage * sizeof(wchar_t), 0, 0);
		CloseHandle(fileHandle);
	}
	delete[] text;
}

void COverlappedWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch HIWORD(wParam)
	{
	case EN_CHANGE:
	{
		textChanged = true;
	}
	}

	switch LOWORD(wParam)
	{
	case ID_VIEW_SETTINGS:
	{
		HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOGBAR), handle, reinterpret_cast<DLGPROC>(dialogProc));
		break;
	}
	}
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
	child.Show(cmdShow);
}

void COverlappedWindow::OnDestroy() {
	DestroyMenu(menu);
	PostQuitMessage(0);
}

LRESULT COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCreate();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_SIZE:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnSize();
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_COMMAND:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnCommand(wParam, lParam);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_CLOSE:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);

		if (window->OnClose()) {
			return DefWindowProc(handle, message, wParam, lParam);
		}
		else {
			return 0;
		}
	}
	case WM_DESTROY:
	{
		COverlappedWindow* window = (COverlappedWindow*)GetWindowLongPtr(handle, GWLP_USERDATA);
		window->OnDestroy();
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}

LRESULT COverlappedWindow::dialogProc(HWND handleDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG: 
	{
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON3:
		{
			EndDialog(handleDlg, 0);
			return TRUE;
		}
		case IDC_BUTTON4:
		{
			EndDialog(handleDlg, 0);
			return FALSE;
		}
		}
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(handleDlg, 0);
		return TRUE;
	}
	default:
		return FALSE;
	}

}
