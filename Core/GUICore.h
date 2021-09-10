#pragma once
#include "../Header.h"
#include "InterfaceContext.h"
#include "../Input/InputManager.h"
#include "../Hooker/D3DHooker.h"
#include <ctime>
#include <functional>
#include "../Data/ZipProvider.h"

using namespace CEGUI;

class GUICore
{
	static DefaultResourceProvider* rp;
	static vector<function<void()>> callers;

	static Window* pRoot;
	static InterfaceContext* pCon;
	static InputManager* pInput;

	static void Initialization(LPDIRECT3DDEVICE9 pDevice);

public:
	GUICore();
	~GUICore();

	static GUIContext* getContext();

	//First stage
	static void LoadZip(string fname);
	//First stage
	static void LoadCryptoZip(const CEGUI::String fname, byte* key, size_t key_size);
	//First stage
	static void LoadResources(string res_dir);
	//Two stage
	static void LoadScheme(string name);
	//Free stage
	static void LoadLayout(string name);

	//Call Async
	static Window* AddLayout(string name);
	//Call Async
	static void AttachCaller(function<void()>);

	//Finish stage
	static void LoadSystem();
};

