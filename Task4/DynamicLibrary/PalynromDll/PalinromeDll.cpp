// PalynromDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "PalindromeDll.h"

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

int Count(const wchar_t* str) 
{
	std::wstringstream iss(str);
	std::vector<std::wstring> splittedStr{ std::istream_iterator<std::wstring, wchar_t>{iss}, std::istream_iterator<std::wstring, wchar_t>{} };
	int result = 0;
	for(auto&& word : splittedStr) {
		std::wstring copyWord(word);
		std::reverse(copyWord.begin(), copyWord.end());
		if(word == copyWord) {
			result++;
		}
	}
	return result;
}

