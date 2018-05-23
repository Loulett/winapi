#pragma once
#include <Windows.h>
#include <vector>
#include <map>

enum Colors {BLACK = 0, WHITE = 1};

struct CheckerBoardField {
	HBRUSH fieldColor;
	bool withChecker = false, isPossibleMove = false, isSelected = false;
	HBRUSH checkerColor;
	int iPossibleMoveParent, jPossibleMoveParent;
	int color;

	CheckerBoardField(HBRUSH newColor) {
		fieldColor = newColor;
	}

	CheckerBoardField(HBRUSH fieldColor_, bool withChecker_, HBRUSH checkerColor_, int color_) {
		fieldColor = fieldColor_;
		withChecker = withChecker_;
		checkerColor = checkerColor_;
		color = color_;
	}
};

class CheckerBoard {
public:
	CheckerBoard();
	static bool RegisterClass();
	bool Create(HWND parentHandle);
	void Show(int cmdShow);
	HWND GetHandle() { return handle; }

protected:
	void OnPaint();
	void OnLButtonDown(int x, int y);

private:
	HWND handle, parentHandle;
	std::vector<std::vector<CheckerBoardField>> fields;
	std::map<int, int> amountOfCheckers = {std::make_pair(Colors::WHITE, 12), std::make_pair(Colors::BLACK, 12)};
	int move = Colors::WHITE;

	void markPossibleMoves(int i, int j);
	void clearPossibleMoves();
	void checkSimpleMove(int iInit, int jInit, int i, int j);
	void checkMoveWithEating(int iInit, int jInit, int i, int j);

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lparam);
};