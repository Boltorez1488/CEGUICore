#include "BP.h"

AwBreakPoint bp;

struct Break
{
	BreakPoint* b;
	DWORD func;
	LPVOID eip;
	bool isEip;
};

vector<Break> breaks;

void ExceptionHandler(EXCEPTION_POINTERS* ep)
{
	for (size_t i = 0; i < breaks.size(); i++)
	{
		Break b = breaks[i];
		if ((DWORD)ep->ExceptionRecord->ExceptionAddress == b.b->TargetAddress)
		{
			if (b.isEip)
			{
				if (b.func != NULL)
				{
					void(*pFunction)(PCONTEXT);
					(FARPROC &)pFunction = (FARPROC)b.func;
					pFunction(ep->ContextRecord);
				}
			}
			else
			{
				if (b.func != NULL)
				{
					void(*pFunction)(PCONTEXT);
					(FARPROC &)pFunction = (FARPROC)b.func;
					pFunction(ep->ContextRecord);
				}
				ep->ContextRecord->Eip = (DWORD)b.eip;
			}
		}
	}
}

BP::BP()
{
	bp.SetExceptionHandler(ExceptionHandler);
}

BP::~BP()
{
}

bool BP::AddHWBreak(DWORD addr, LPVOID pEip, LPVOID func)
{
	int dbg = -1;
	bool db[4] = { false };
	
	for (size_t i = 0; i < breaks.size(); i++)
	{
		switch (breaks[i].b->DebugRegister)
		{
		case 0:
			db[0] = true;
			break;
		case 1:
			db[1] = true;
			break;
		case 2:
			db[2] = true;
			break;
		case 3:
			db[3] = true;
			break;
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (db[i] != true)
		{
			dbg = i;
			break;
		}
	}

	if (dbg == -1)
	{
		//printf("HardwareBP is End, Place INT3!\n");
		return AddINT3Break(addr, pEip, func);
	}

	BreakPoint* buff = new BreakPoint;
	Break b;

	buff->TargetAddress = addr;
	buff->DebugRegister = dbg;

	b.eip = pEip;
	b.func = (DWORD)func;
	b.isEip = false;
	b.b = buff;
	breaks.push_back(b);

	return bp.SetHWBreakPoint(buff);
}

bool BP::AddEipHWBreak(DWORD addr, LPVOID func)
{
	int dbg = -1;
	bool db[4] = { false };

	for (size_t i = 0; i < breaks.size(); i++)
	{
		switch (breaks[i].b->DebugRegister)
		{
		case 0:
			db[0] = true;
			break;
		case 1:
			db[1] = true;
			break;
		case 2:
			db[2] = true;
			break;
		case 3:
			db[3] = true;
			break;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (db[i] != true)
		{
			dbg = i;
			break;
		}
	}

	if (dbg == -1)
	{
		//printf("HardwareBP is End, Place INT3!\n");
		return AddEipINT3Break(addr, func);
	}

	BreakPoint* buff = new BreakPoint;
	Break b;

	buff->TargetAddress = addr;
	buff->DebugRegister = dbg;

	b.eip = NULL;
	b.func = (DWORD)func;
	b.isEip = true;
	b.b = buff;
	breaks.push_back(b);

	return bp.SetHWBreakPoint(buff);
}

bool BP::AddINT3Break(DWORD addr, LPVOID pEip, LPVOID func)
{
	BreakPoint* buff = new BreakPoint;
	Break b;

	buff->TargetAddress = addr;

	b.eip = pEip;
	b.func = (DWORD)func;
	b.isEip = false;
	b.b = buff;
	breaks.push_back(b);

	return bp.SetINT3BreakPoint(buff);
}

bool BP::AddEipINT3Break(DWORD addr, LPVOID func)
{
	BreakPoint* buff = new BreakPoint;
	Break b;

	buff->TargetAddress = addr;

	b.eip = NULL;
	b.func = (DWORD)func;
	b.isEip = true;
	b.b = buff;
	breaks.push_back(b);

	return bp.SetINT3BreakPoint(buff);
}

bool BP::ClearBP(DWORD addr)
{
	for (size_t i = 0; i < breaks.size(); i++)
	{
		BreakPoint* bb = breaks[i].b;
		if (bb->TargetAddress == addr)
		{
			if (bb->DebugRegister = kError)
				bp.ClearINT3BreakPoint(bb);
			else
				bp.ClearINT3BreakPoint(bb);
			breaks.erase(breaks.begin() + i);
			return true;
		}
	}
	return false;
}
