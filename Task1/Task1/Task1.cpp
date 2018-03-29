// Task1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <strsafe.h>
#include <vector>


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

void task1() {
	size_t n = 0;
	while(true) {
		PVOID p = VirtualAlloc(NULL, 64 * 1024, MEM_RESERVE, PAGE_READWRITE);
		if(!p) {
			printError();
			break;
		}
		n++;
	}
	std::cout << "Memory reserved: " << n * 64 / 1024. / 1024. << "Gb" << std::endl; //3.9 Gb
	system("pause");
}

void task2() {
	while(true) {
		int memSize = rand() % 1024 + 1;
		int flag = rand() % 2;
		PVOID p = nullptr;
		if(flag == 0) {
			p = VirtualAlloc(NULL, memSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_GUARD | PAGE_READWRITE);
		}else {
			p = VirtualAlloc(NULL, memSize, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
		}

		if(!p) {
			printError();
			break;
		}
	}
	system("pause");
}

void task3() {
	size_t n = 0;
	HANDLE handle = CreateEvent(NULL, TRUE, TRUE, NULL);
	HANDLE copyHandle;
	while(true) {
		DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(), &copyHandle, DUPLICATE_SAME_ACCESS, FALSE, 0);

		if (!copyHandle) {
			printError();
			break;
		}

		n++;
	}
	std::cout << n; // 16711645
	std::cin >> n;
	system("pause");
}

void task4() {
	HPEN pen;
	size_t n = 0;
	while(true) {
		pen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));

		if(!pen) {
			printError();
			break;
		}

		n++;
	}
	std::cout << n; // 9996
	std::cin >> n;
	system("pause");
}

int n = 0;
void EmptyFunction() {
	n++;
	EmptyFunction();
}

void task5() {
	EmptyFunction();
} // 214 байт

int m = 0;

void EmptyFunction1() {
	m++;
	int s = 1234;
	EmptyFunction1();
}

void task6() {
	EmptyFunction1();
} // 227 байт

int main()
{
	//task1();
	//task2();
	//task3();
	//task4();
	//task5();
	task6();
    return 0;
}

