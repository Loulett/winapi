#include "CheckerBoard.h"
#include "BaseWindow.h"

CheckerBoard::CheckerBoard() {
	for (size_t i = 0; i < 8; i++) {
		std::vector<CheckerBoardField> line = {};
		for (size_t j = 0; j < 8; j++) {
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) {
				line.push_back(CheckerBoardField(CreateSolidBrush(RGB(222, 184, 135))));
			}
			else {
				if (i == 0 || i == 1 || i == 2) {
					line.push_back(CheckerBoardField(CreateSolidBrush(RGB(160, 82, 45)), true, CreateSolidBrush(RGB(0, 0, 0)), Colors::BLACK));
				}
				else if (i == 5 || i == 6 || i == 7) {
					line.push_back(CheckerBoardField(CreateSolidBrush(RGB(160, 82, 45)), true, CreateSolidBrush(RGB(255, 255, 255)), Colors::WHITE));
				}
				else {
					line.push_back(CheckerBoardField(CreateSolidBrush(RGB(160, 82, 45))));
				}
			}
		}
		fields.push_back(line);
	}
}

bool CheckerBoard::RegisterClass() {
	WNDCLASSEX ws;
	::ZeroMemory(&ws, sizeof(ws));
	ws.cbSize = sizeof(WNDCLASSEX);
	ws.lpfnWndProc = CheckerBoard::windowProc;
	ws.hInstance = GetModuleHandle(0);
	ws.lpszClassName = L"CheckerBoard";
	return (::RegisterClassEx(&ws) != 0);
}

bool CheckerBoard::Create(HWND parentHandle) {
	handle = CreateWindowEx(0,
		L"CheckerBoard", L"Child Window", WS_CHILD | WS_BORDER, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, 0, GetModuleHandle(0), this);
	this->parentHandle = parentHandle;
	return (handle != 0);
}

void CheckerBoard::Show(int cmdShow) {
	ShowWindow(handle, cmdShow);
}

void CheckerBoard::OnPaint() {
	PAINTSTRUCT ps;
	RECT rect;
	
	HDC dc = ::BeginPaint(handle, &ps);
	GetClientRect(handle, &rect);
	HDC bufferDC = CreateCompatibleDC(dc);
	HBITMAP buffer = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
	HBITMAP oldBuffer = (HBITMAP)SelectObject(bufferDC, buffer);

	int stepH = (rect.bottom - rect.top) / 8;
	int stepW = (rect.right - rect.left) / 8;

	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			SelectObject(bufferDC, fields[i][j].fieldColor);
			Rectangle(bufferDC, j*stepW, i*stepH, (j + 1)*stepW, (i + 1)*stepH);
			if (fields[i][j].withChecker) {
				SelectObject(bufferDC, fields[i][j].checkerColor);
				Ellipse(bufferDC, j*stepW, i*stepH, (j + 1)*stepW, (i + 1)*stepH);
			}
			if (fields[i][j].isPossibleMove) {
				SelectObject(bufferDC, CreateSolidBrush(RGB(255, 255, 255)));
				Ellipse(bufferDC, j*stepW + stepW / 4, i*stepH + stepH / 4, (j + 1)*stepW - stepW / 4, (i + 1)*stepH - stepH / 4);
			}
		}
	}

	BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, bufferDC, 0, 0, SRCCOPY);
	SelectObject(bufferDC, oldBuffer);
	DeleteObject(buffer);
	DeleteDC(bufferDC);

	::EndPaint(handle, &ps);
}

void CheckerBoard::clearPossibleMoves() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			fields[i][j].isPossibleMove = false;
		}
	}
}

void CheckerBoard::checkSimpleMove(int iInit, int jInit, int i, int j) {
	if (i > -1 && j > -1 && i < 8 && j < 8 && !fields[i][j].withChecker) {
		fields[i][j].isPossibleMove = true;
		fields[i][j].iPossibleMoveParent = iInit;
		fields[i][j].jPossibleMoveParent = jInit;
	}
}

void CheckerBoard::checkMoveWithEating(int iInit, int jInit, int i, int j) {
	if (i > -1 && j > -1 && i < 8 && j < 8 && !fields[i][j].withChecker && fields[(i + iInit) / 2][(j + jInit) / 2].withChecker && fields[(i + iInit) / 2][(j + jInit) / 2].color != fields[iInit][jInit].color) {
		fields[i][j].isPossibleMove = true;
		fields[i][j].iPossibleMoveParent = iInit;
		fields[i][j].jPossibleMoveParent = jInit;
	}

}

void CheckerBoard::markPossibleMoves(int i, int j) {
	if (fields[i][j].color == Colors::WHITE) {
		checkSimpleMove(i, j, i - 1, j - 1);
		checkSimpleMove(i, j, i - 1, j + 1);
		checkMoveWithEating(i, j, i - 2, j - 2);
		checkMoveWithEating(i, j, i - 2, j + 2);
		checkMoveWithEating(i, j, i + 2, j - 2);
		checkMoveWithEating(i, j, i + 2, j + 2);
	}

	if (fields[i][j].color == Colors::BLACK) {
		checkSimpleMove(i, j, i + 1, j - 1);
		checkSimpleMove(i, j, i + 1, j + 1);
		checkMoveWithEating(i, j, i + 2, j - 2);
		checkMoveWithEating(i, j, i + 2, j + 2);
		checkMoveWithEating(i, j, i - 2, j - 2);
		checkMoveWithEating(i, j, i - 2, j + 2);
	}
}

void CheckerBoard::OnLButtonDown(int x, int y) {
	RECT rect;
	GetClientRect(handle, &rect);

	int stepH = (rect.bottom - rect.top) / 8;
	int stepW = (rect.right - rect.left) / 8;

	int j = x / stepW, i = y / stepH;
	if (fields[i][j].withChecker && fields[i][j].color == move) {
		clearPossibleMoves();
		fields[i][j].isSelected = !fields[i][j].isSelected;
		if (fields[i][j].isSelected) {
			markPossibleMoves(i, j);
		}
	}
	if (fields[i][j].isPossibleMove) {
		clearPossibleMoves();
		fields[i][j].withChecker = true;
		fields[i][j].color = fields[fields[i][j].iPossibleMoveParent][fields[i][j].jPossibleMoveParent].color;
		fields[i][j].checkerColor = fields[fields[i][j].iPossibleMoveParent][fields[i][j].jPossibleMoveParent].checkerColor;
		fields[fields[i][j].iPossibleMoveParent][fields[i][j].jPossibleMoveParent].withChecker = false;
		if (abs(fields[i][j].iPossibleMoveParent - i) == 2) {
			fields[(fields[i][j].iPossibleMoveParent + i) / 2][(fields[i][j].jPossibleMoveParent + j) / 2].withChecker = false;
			int eatenColor = fields[(fields[i][j].iPossibleMoveParent + i) / 2][(fields[i][j].jPossibleMoveParent + j) / 2].color;
			amountOfCheckers[eatenColor] -= 1;
			if (amountOfCheckers[eatenColor] == 0) {
				BaseWindow* window = (BaseWindow*)GetWindowLongPtr(parentHandle, GWLP_USERDATA);
				window->EndGame(eatenColor);
			}
		}

		if (move == Colors::BLACK) {
			move = Colors::WHITE;
		}
		else {
			move = Colors::BLACK;
		}
	}
	InvalidateRect(handle, NULL, FALSE);
}

LRESULT CheckerBoard::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		void* ptr = pcs->lpCreateParams;
		::SetWindowLongPtr(handle, GWLP_USERDATA, PtrToUlong(ptr));
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_PAINT:
	{
		InvalidateRect(handle, NULL, FALSE);
		CheckerBoard* board = (CheckerBoard*)GetWindowLongPtr(handle, GWLP_USERDATA);
		board->OnPaint();
		return DefWindowProc(handle, message, wParam, lParam);
		InvalidateRect(handle, NULL, FALSE);
	}
	case WM_LBUTTONDOWN:
	{
		CheckerBoard* board = (CheckerBoard*)GetWindowLongPtr(handle, GWLP_USERDATA);
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		board->OnLButtonDown(xPos, yPos);
		return DefWindowProc(handle, message, wParam, lParam);
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(handle, message, wParam, lParam);
	}
}