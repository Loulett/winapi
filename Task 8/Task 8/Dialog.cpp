#include "Dialog.h"
#include "resource.h"
#include "COverlappedWindow.h"
#include <Commctrl.h>
#include <strsafe.h>

void printError()
{
	LPTSTR lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		0, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	// Display the error message 

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)TEXT("GetProcessId")) + 40) * sizeof(TCHAR));

	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		TEXT("GetProcessId"), dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

HWND Dialog::GetHandle() {
	return handle;
}

void Dialog::Create(HINSTANCE hInstance, HWND pHandle, HWND ecHandle) {
	parentHandle = pHandle;
	editControlHandle = ecHandle;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOGBAR), pHandle, (DLGPROC)dialogProc, (LPARAM)this);
}

void Dialog::OnCreate(HWND handleDlg) {
	handle = handleDlg;
	SendMessage(GetDlgItem(handle, IDC_SLIDER1), TBM_SETRANGE, TRUE, MAKELONG(8, 72));
	SendMessage(GetDlgItem(handle, IDC_SLIDER2), TBM_SETRANGE, TRUE, MAKELONG(0, 255));

	LOGFONT logFont;
	HFONT font = (HFONT)(SendMessage(editControlHandle, WM_GETFONT, 0, 0));
	GetObject(font, sizeof(logFont), &logFont);

	COverlappedWindow* parent = (COverlappedWindow*)GetWindowLongPtr(parentHandle, GWLP_USERDATA);
	oldSettings.font = logFont;
	oldSettings.opacity = parent->GetOpacity();
	oldSettings.fontColor = parent->GetFontColor();
	oldSettings.backGroundColor = parent->GetBgColor();

	newSettings.font = logFont;
	newSettings.opacity = parent->GetOpacity();
	newSettings.fontColor = parent->GetFontColor();
	newSettings.backGroundColor = parent->GetBgColor();

	SendMessage(GetDlgItem(handle, IDC_SLIDER1), TBM_SETPOS, true, (LPARAM)logFont.lfHeight);
	SendMessage(GetDlgItem(handle, IDC_SLIDER2), TBM_SETPOS, true, newSettings.opacity);
}

void Dialog::SelectColor(DWORD& targetColor) {
	CHOOSECOLOR cc = {};
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = parentHandle;
	cc.rgbResult = targetColor;
	cc.lpCustColors = palitre;
	cc.Flags = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);
	targetColor = cc.rgbResult;
}

void Dialog::OnScroll(LPARAM lParam) {
	HWND commandHandle = (HWND)(lParam);
	if (commandHandle == GetDlgItem(handle, IDC_SLIDER1)) {
		newSettings.font.lfHeight = SendMessage(GetDlgItem(handle, IDC_SLIDER1), TBM_GETPOS, 0, 0);
	}
	else if (commandHandle == GetDlgItem(handle, IDC_SLIDER2)) {
		newSettings.opacity = (BYTE)SendMessage(GetDlgItem(handle, IDC_SLIDER2), TBM_GETPOS, 0, 0);
	}
}

void Dialog::OnCheck() {
	preview = !preview;
}

void Dialog::updateSettings(Settings& settings) {
	COverlappedWindow* parent = (COverlappedWindow*)GetWindowLongPtr(parentHandle, GWLP_USERDATA);
	parent->SetBgColor(settings.backGroundColor);
	parent->SetFontColor(settings.fontColor);
	parent->SetOpacity(settings.opacity);

	bool res = true;
	res = SetLayeredWindowAttributes(parentHandle, RGB(0, 0, 0), settings.opacity, LWA_ALPHA);
	if (! res)
	{
		printError();
	}

	HFONT actualFont = CreateFontIndirect(&settings.font);
	SendMessage(editControlHandle, WM_SETFONT, reinterpret_cast<WPARAM>(actualFont), TRUE);
}

void Dialog::OnOk() {
	updateSettings(newSettings);
}

void Dialog::OnClose() {
	updateSettings(oldSettings);
}

void Dialog::tryPreview() {
	if (preview) {
		updateSettings(newSettings);
	}
}

LRESULT Dialog::dialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
	{
		Dialog* dialog = (Dialog*)lParam;
		::SetWindowLongPtr(handle, GWLP_USERDATA, (LONG)dialog);
		dialog->OnCreate(handle);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON3:
		{
			Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
			dialog->OnOk();
			EndDialog(handle, 0);
			break;
		}
		case IDC_BUTTON4:
		{
			Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
			dialog->OnClose();
			EndDialog(handle, 0);
			break;
		}
		case IDC_BUTTON1:
		{
			Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
			dialog->SelectColor(dialog->newSettings.backGroundColor);
			break;
		}
		case IDC_BUTTON2:
		{
			Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
			dialog->SelectColor(dialog->newSettings.fontColor);
			break;
		}
		case IDC_CHECK1:
		{
			Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
			dialog->OnCheck();
		}
		}
		Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
		dialog->tryPreview();
		break;
	}
	case WM_HSCROLL:
	{
		Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
		dialog->OnScroll(lParam);
		dialog->tryPreview();
		return FALSE;
	}
	case WM_CLOSE:
	{
		Dialog* dialog = (Dialog*)GetWindowLongPtr(handle, GWLP_USERDATA);
		dialog->OnClose();
		EndDialog(handle, 0);
		return TRUE;
	}
	default:
		return FALSE;
	}

}