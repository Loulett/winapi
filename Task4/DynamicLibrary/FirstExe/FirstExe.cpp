// FirstExe.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "PalindromeDll.h"

#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>

typedef int(__cdecl *MYPROC)(const wchar_t*);

int main(int argc, char** argv)
{
	HINSTANCE PalindromeDll;
	if((PalindromeDll = LoadLibrary(TEXT("PalindromeDll.dll"))) == NULL) {
		return 1;
	}
	MYPROC ProcAdd = (MYPROC)GetProcAddress(PalindromeDll, "Count");

	std::wfstream file(argv[1]);
	std::wstring string;
	getline(file, string);

	TCHAR sz[100];
	wsprintf(sz, TEXT("%d"), (ProcAdd)(string.c_str()));
	MessageBox(NULL, sz, TEXT("Calculation"), MB_OK);
	FreeLibrary(PalindromeDll);

	file.close();
    return 0;
}

