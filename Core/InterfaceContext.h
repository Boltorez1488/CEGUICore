#pragma once
#include "../Header.h"

using namespace CEGUI;

class InterfaceContext
{
	GUIContext* d_guiContext;
	bool isInit;

	System* sys;

	LPDIRECT3DDEVICE9 pDevice;
	Direct3D9Renderer* pRender;

public:
	InterfaceContext();
	~InterfaceContext();

	void SetDevice(LPDIRECT3DDEVICE9 pD);
	void Init(ResourceProvider* rp);
	void Render() const;

	BOOL IsInit() const;
	GUIContext* getContext() const;
	Direct3D9Renderer* getRender() const;
};

