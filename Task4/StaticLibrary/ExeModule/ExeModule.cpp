// Author: Alexandra Iglina
//
// ExeModule.cpp: Connection with .lib library
//

#include "stdafx.h"
#include "Palindrome.h"

#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>


int main(int argc, char** argv)
{
	std::wfstream file(argv[1]);
	std::wstring string;
	getline(file, string);

	TCHAR sz[100];
	wsprintf(sz, TEXT("%d"), Count(string.c_str()));
	MessageBox(NULL, sz, TEXT("Calculation"), MB_OK);

	file.close();
	return 0;
}

