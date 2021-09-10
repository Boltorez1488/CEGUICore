#pragma once
#include "AwBreakPoint.h"
#include <vector>

using namespace std;

class BP
{
public:
	BP();
	~BP();

	bool AddHWBreak(DWORD addr, LPVOID pEip, LPVOID func = NULL);
	bool AddEipHWBreak(DWORD addr, LPVOID func = NULL);

	bool AddINT3Break(DWORD addr, LPVOID pEip, LPVOID func = NULL);
	bool AddEipINT3Break(DWORD addr, LPVOID func = NULL);

	bool ClearBP(DWORD addr);
};

