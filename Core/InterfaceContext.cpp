#include "InterfaceContext.h"
#include <ctime>

InterfaceContext::InterfaceContext()
{
	d_guiContext = NULL;
	isInit = false;
	sys = NULL;
	pDevice = NULL;
	pRender = NULL;
}

InterfaceContext::~InterfaceContext()
{
}

void InterfaceContext::SetDevice(LPDIRECT3DDEVICE9 pD)
{
	pDevice = pD;
}

void InterfaceContext::Init(ResourceProvider* rp)
{
	pRender = &Direct3D9Renderer::create(pDevice);
	if(rp != NULL)
		System::create(*pRender, rp);
	else
		System::create(*pRender);

	sys = &CEGUI::System::getSingleton();

	d_guiContext = &sys->createGUIContext(pRender->getDefaultRenderTarget());

	isInit = true;
}

extern clock_t mLastTimeInjection;
void InterfaceContext::Render() const
{
	pRender->beginRendering();
	clock_t now = clock();
	d_guiContext->injectTimePulse((now - mLastTimeInjection) * 0.001f);
	sys->injectTimePulse((now - mLastTimeInjection) * 0.001f);
	mLastTimeInjection = clock();
	d_guiContext->draw();
	pRender->endRendering();
	WindowManager::getSingleton().cleanDeadPool();
}

BOOL InterfaceContext::IsInit() const
{
	return isInit;
}

GUIContext* InterfaceContext::getContext() const
{
	return d_guiContext;
}

Direct3D9Renderer* InterfaceContext::getRender() const
{
	return pRender;
}
