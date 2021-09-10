#pragma once
#include <Windows.h>
#include "asmjit\asmjit.h"
#include "Mhook\mhook-lib\mhook.h"
#include <vector>

using namespace asmjit;
using namespace asmjit::x86;

extern JitRuntime runtime;
#define ASM(varname) X86Assembler varname(&runtime)

class Patcher
{
	struct ABSD
	{
		LPVOID addr;
		BYTE * ori;
		int Size;
	};
	std::vector<ABSD> addrs;

public:
	BOOL PlaceJMP(DWORD pSrc, void *pDest, int nNops = 0);
	BOOL PlaceJMP(LPVOID pSrc, void *pDest, int nNops = 0);
	BOOL ClearJMP(DWORD pSrc);
	BOOL ClearJMP(LPVOID pSrc); 

	LPVOID GetAsmJit(X86Assembler * x86);

	BOOL PlaceBytes(DWORD pSrc, BYTE* bytes, int Size);
	BOOL PlaceBytes(LPVOID pSrc, BYTE* bytes, int Size);
	BOOL SetNOPS(DWORD pSrc, int size);
	BOOL SetNOPS(LPVOID pSrc, int size);

	BOOL PlaceJit(DWORD pSrc, X86Assembler * x86, int Nops = 0);
	BOOL PlaceJit(LPVOID pSrc, X86Assembler * x86, int Nops = 0);

	BOOL PlaceCode(DWORD pSrc, X86Assembler * x86);
	BOOL PlaceCode(LPVOID pSrc, X86Assembler * x86);

	BOOL PlaceCode(DWORD pSrc, BYTE* bytes, size_t size);
	BOOL PlaceCode(LPVOID pSrc, BYTE* bytes, size_t size);

	BOOL PlaceCode(DWORD pSrc, std::vector<BYTE> bytes);
	BOOL PlaceCode(LPVOID pSrc, std::vector<BYTE> bytes);

	BOOL PlaceCode(DWORD pSrc, char* bytes, size_t size);
	BOOL PlaceCode(LPVOID pSrc, char* bytes, size_t size);

	BYTE * Patcher::GetOriBytes(LPVOID pSrc); //If address include in struct
	BYTE * Patcher::GetOriBytes(DWORD pSrc); //If address include in struct

	BOOL API_Hook(LPVOID ptr, LPVOID dst); //For void*
	BOOL API_Unhook(LPVOID hooked);

	void* __cdecl memcpy(void* dst, void* ptr, size_t size);

	Patcher();
	~Patcher();
};

