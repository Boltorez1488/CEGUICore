#pragma once
#include "../Header.h"
#include "../tools/Patcher.h"
#include "../Input/InputManager.h"
#include "../Core/InterfaceContext.h"

using namespace std;

class D3DHooker
{
	static ResourceProvider* RP;
	static void(*Initializator)(LPDIRECT3DDEVICE9 pDevice);
	static InputManager *d_Msg;
	static InterfaceContext* pContext;
	static LPDIRECT3DDEVICE9 nm_pD3Ddev, pDevice;
	static bool isHooked;

	static vector<void(*)(LPDIRECT3DDEVICE9)> pEnds;
	static vector<void(*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)> preResets, postResets;
	static vector<BOOL(WINAPI *)(const MSG *lpMsg)> msgs;

	static LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }
	static void DX_Init(DWORD* table);
	static PBYTE HookVTable(PDWORD* dwVTable, PBYTE dwHook, INT Index);
	static DWORD WINAPI VMT_Patching(LPVOID  Param);

	static HRESULT(WINAPI *pEndScene)(LPDIRECT3DDEVICE9 pDevice);
	static HRESULT APIENTRY EndScene(LPDIRECT3DDEVICE9 pD3D9);

	static HRESULT(WINAPI* pReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static HRESULT WINAPI Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

	static LRESULT(WINAPI *pMsg)(const MSG *lpMsg);
	static LRESULT APIENTRY Msg(const MSG *lpMsg);

	static void LoopFunction();

public:
	D3DHooker();
	~D3DHooker();

	static void AttachRP(ResourceProvider* r);
	static void AttachInput(InputManager* manager);
	static void AttachContext(InterfaceContext* context);

	static void Hook(void(*init)(LPDIRECT3DDEVICE9 pDevice));
	static LPDIRECT3DDEVICE9 GetDevice();

	static void AttachMsgReciever(BOOL(WINAPI * rec)(const MSG *lpMsg));
	static void AttachEndScene(void(*scene)(LPDIRECT3DDEVICE9));
	static void AttachPreReset(void(*pre)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters));
	static void AttachPostReset(void(*post)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters));
};

