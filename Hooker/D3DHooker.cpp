#include "D3DHooker.h"

InputManager *D3DHooker::d_Msg;
InterfaceContext* D3DHooker::pContext;
LPDIRECT3DDEVICE9 D3DHooker::nm_pD3Ddev, D3DHooker::pDevice;
bool D3DHooker::isHooked;

void(*D3DHooker::Initializator)(LPDIRECT3DDEVICE9 pDevice);

vector<void(*)(LPDIRECT3DDEVICE9)> D3DHooker::pEnds;
vector<void(*)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)> D3DHooker::preResets, D3DHooker::postResets;
vector<BOOL(WINAPI *)(const MSG *lpMsg)> D3DHooker::msgs;

HRESULT(WINAPI *D3DHooker::pEndScene)(LPDIRECT3DDEVICE9 pDevice);
HRESULT(WINAPI* D3DHooker::pReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
LRESULT(WINAPI *D3DHooker::pMsg)(const MSG *lpMsg);

void D3DHooker::DX_Init(DWORD* table)
{
	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
	LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	LPDIRECT3DDEVICE9 pd3dDevice;
	pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);

	auto pVTable = reinterpret_cast<DWORD*>(pd3dDevice);
	pVTable = reinterpret_cast<DWORD*>(pVTable[0]);

	table[0] = pVTable[42];
	table[2] = pVTable[16];

	DestroyWindow(hWnd);
}

PBYTE D3DHooker::HookVTable(PDWORD* dwVTable, PBYTE dwHook, INT Index)
{
	DWORD dwOld = 0;
	VirtualProtect(static_cast<void*>((*dwVTable) + (Index * 4)), 4, PAGE_EXECUTE_READWRITE, &dwOld);
	auto pOrig = reinterpret_cast<PBYTE>((*dwVTable)[Index]);
	(*dwVTable)[Index] = reinterpret_cast<DWORD>(dwHook);
	VirtualProtect(static_cast<void*>((*dwVTable) + (Index * 4)), 4, dwOld, &dwOld);

	return pOrig;
}

DWORD D3DHooker::VMT_Patching(LPVOID Param)
{
	while (1)
	{
		Sleep(100);

		HookVTable(reinterpret_cast<PDWORD*>(nm_pD3Ddev), reinterpret_cast<PBYTE>(EndScene), 42);
	}
	return 1;
}

ResourceProvider* D3DHooker::RP;
HRESULT D3DHooker::EndScene(LPDIRECT3DDEVICE9 pD3D9)
{
	if (pDevice == NULL)
		pDevice = pD3D9;
	if(pContext != NULL )
	{
		if(!pContext->IsInit())
		{
			pContext->SetDevice(pD3D9);
			pContext->Init(RP);
			d_Msg->SetContext(pContext->getContext());
			Initializator(pD3D9);
		}
		pContext->Render();
	}

	for(auto i = pEnds.begin(); i != pEnds.end(); ++i)
	{
		auto p = *i;
		p(pD3D9);
	}
	return pEndScene(pD3D9);
}

HRESULT D3DHooker::Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	for (auto i = preResets.begin(); i != preResets.end(); ++i)
	{
		auto p = *i;
		p(pDevice, pPresentationParameters);
	}

	if(pContext != NULL && pContext->IsInit())
	{
		pContext->getRender()->preD3DReset();
	}
	HRESULT iReturnValue = pReset(pDevice, pPresentationParameters);
	if (pContext != NULL && pContext->IsInit())
	{
		pContext->getRender()->postD3DReset();
	}

	for(auto i = postResets.begin(); i != postResets.end(); ++i)
	{
		auto p = *i;
		p(pDevice, pPresentationParameters);
	}
	return iReturnValue;
}

LRESULT D3DHooker::Msg(const MSG* lpMsg)
{
	bool flag = false;
	if(d_Msg != NULL && pContext != NULL && pContext->IsInit())
	{
		flag = d_Msg->TranslateMessage(lpMsg);
	}
	for(auto i = msgs.begin(); i != msgs.end(); ++i)
	{
		auto p = *i;
		if (p(lpMsg))
			flag = true;
	}
	if (flag)
		return 0;
	return pMsg(lpMsg);
}

void D3DHooker::LoopFunction()
{
	if (isHooked == false)
	{
		DWORD VTable[3] = { 0 };
		while (GetModuleHandleA("d3d9.dll") == NULL)
		{
			Sleep(250);
		}
		DX_Init(VTable);
		*reinterpret_cast<PDWORD>(&pEndScene) = VTable[0];
		*reinterpret_cast<PDWORD>(&pReset) = VTable[2];
		Mhook_SetHook(reinterpret_cast<PVOID*>(&pEndScene), EndScene);
		Mhook_SetHook(reinterpret_cast<PVOID*>(&pReset), Reset);
		Mhook_SetHook(reinterpret_cast<PVOID*>(&pMsg), Msg);

		while (!nm_pD3Ddev)
		{
			Sleep(50);
		}
		Mhook_Unhook(reinterpret_cast<PVOID*>(&EndScene));
		Mhook_Unhook(reinterpret_cast<PVOID*>(&Reset));
		Mhook_Unhook(reinterpret_cast<PVOID*>(&Msg));
		CreateThread(NULL, 0, &VMT_Patching, NULL, 0, NULL);
		isHooked = !isHooked;
	}
}

D3DHooker::D3DHooker()
{
	isHooked = false;
	pMsg = DispatchMessageA;
	d_Msg = NULL;
	pDevice = NULL;
	pContext = NULL;
	Initializator = NULL;
	RP = NULL;
}

D3DHooker::~D3DHooker()
{
}

void D3DHooker::AttachRP(ResourceProvider* r)
{
	RP = r;
}

void D3DHooker::AttachInput(InputManager * manager)
{
	d_Msg = manager;
}

void D3DHooker::AttachContext(InterfaceContext* context)
{
	pContext = context;
}

void D3DHooker::Hook(void(*init)(LPDIRECT3DDEVICE9 pDevice))
{
	Initializator = init;
	CreateThread(0, 0, LPTHREAD_START_ROUTINE(LoopFunction), 0, 0, 0);
}

LPDIRECT3DDEVICE9 D3DHooker::GetDevice()
{
	return pDevice;
}

void D3DHooker::AttachMsgReciever(BOOL(__stdcall * rec)(const MSG* lpMsg))
{
	msgs.push_back(rec);
}

void D3DHooker::AttachEndScene(void(* scene)(LPDIRECT3DDEVICE9))
{
	pEnds.push_back(scene);
}

void D3DHooker::AttachPreReset(void(* pre)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters))
{
	preResets.push_back(pre);
}

void D3DHooker::AttachPostReset(void(* post)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters))
{
	postResets.push_back(post);
}
