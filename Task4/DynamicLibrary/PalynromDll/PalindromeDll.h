#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>

#ifdef PALINDROMEDLL_EXPORTS
#define PALINDROME_API extern "C" __declspec(dllexport) 
#else
#define PALINDROME_API extern "C" __declspec(dllimport) 
#endif

PALINDROME_API int Count(const wchar_t* str);
