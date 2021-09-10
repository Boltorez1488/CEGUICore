#include "GUICore.h"

clock_t mLastTimeInjection = clock();

DefaultResourceProvider* GUICore::rp;
vector<function<void()>> GUICore::callers;

Window* GUICore::pRoot;
InterfaceContext* GUICore::pCon;
InputManager* GUICore::pInput;

void GUICore::Initialization(LPDIRECT3DDEVICE9 pDevice)
{
	for(auto i = callers.begin(); i != callers.end(); ++i)
	{
		auto p = *i;
		p();
	}
	callers.clear();
}

GUICore::GUICore()
{
	rp = NULL;
	pRoot = NULL;
	pCon = NULL;
	pInput = NULL;
}

GUICore::~GUICore()
{
}

GUIContext* GUICore::getContext()
{
	return pCon->getContext();
}

void GUICore::LoadZip(string fname)
{
	rp = new ZipProvider(fname);
	callers.push_back([]()
	{
		rp->setResourceGroupDirectory("schemes", "schemes/");
		rp->setResourceGroupDirectory("imagesets", "imagesets/");
		rp->setResourceGroupDirectory("fonts", "fonts/");
		rp->setResourceGroupDirectory("layouts", "layouts/");
		rp->setResourceGroupDirectory("looknfeels", "looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "lua_scripts/");

		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WindowManager::setDefaultResourceGroup("layouts");
		ScriptModule::setDefaultResourceGroup("lua_scripts");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		AnimationManager::setDefaultResourceGroup("animations");

		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
	});
}

void GUICore::LoadCryptoZip(const CEGUI::String fname, byte * key, size_t key_size)
{
	rp = new ZipProvider(fname, key, key_size);
	callers.push_back([]()
	{
		rp->setResourceGroupDirectory("schemes", "schemes/");
		rp->setResourceGroupDirectory("imagesets", "imagesets/");
		rp->setResourceGroupDirectory("fonts", "fonts/");
		rp->setResourceGroupDirectory("layouts", "layouts/");
		rp->setResourceGroupDirectory("looknfeels", "looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "lua_scripts/");

		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WindowManager::setDefaultResourceGroup("layouts");
		ScriptModule::setDefaultResourceGroup("lua_scripts");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		AnimationManager::setDefaultResourceGroup("animations");

		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
	});
}

void GUICore::LoadResources(string dir)
{
	rp = new DefaultResourceProvider;
	callers.push_back([dir]()
	{
		rp->setResourceGroupDirectory("schemes", dir + "/schemes/");
		rp->setResourceGroupDirectory("imagesets", dir + "/imagesets/");
		rp->setResourceGroupDirectory("fonts", dir + "/fonts/");
		rp->setResourceGroupDirectory("layouts", dir + "/layouts/");
		rp->setResourceGroupDirectory("looknfeels", dir + "/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", dir + "/lua_scripts/");

		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WindowManager::setDefaultResourceGroup("layouts");
		ScriptModule::setDefaultResourceGroup("lua_scripts");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		AnimationManager::setDefaultResourceGroup("animations");

		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");

		pCon->getContext()->setMouseButtonClickTimeout(0);
		pCon->getContext()->setMouseButtonMultiClickTimeout(0);
	});
}

void GUICore::LoadScheme(string name)
{
	callers.push_back([name]()
	{
		RawDataContainer cont;
		rp->loadRawDataContainer(name, cont, "schemes");
		SchemeManager::getSingleton().createFromContainer(cont);
	});
}

void GUICore::LoadLayout(string name)
{
	callers.push_back([name]()
	{
		RawDataContainer cont;
		rp->loadRawDataContainer(name, cont, "layouts");
		WindowManager& winMgr(WindowManager::getSingleton());
		//pRoot = winMgr.createWindow("DefaultWindow", "HUDDemoRoot");
		//pRoot->addChild(winMgr.loadLayoutFromContainer(cont));
		pRoot = winMgr.loadLayoutFromContainer(cont);
		pCon->getContext()->setRootWindow(pRoot);
		/*if(!pRoot)
		{
			pRoot = winMgr.loadLayoutFromContainer(cont);
			pCon->getContext()->setRootWindow(pRoot);
		}
		else
		{
			
		}*/
	});
}

Window* GUICore::AddLayout(string name)
{
	RawDataContainer cont;
	rp->loadRawDataContainer(name, cont, "layouts");
	WindowManager& winMgr(WindowManager::getSingleton());
	return winMgr.loadLayoutFromContainer(cont);
}

void GUICore::AttachCaller(function<void()> func)
{
	callers.push_back(func);
}

void GUICore::LoadSystem()
{
	pCon = new InterfaceContext;
	pInput = new InputManager;
	D3DHooker();
	D3DHooker::AttachRP(rp);
	D3DHooker::AttachContext(pCon);
	D3DHooker::AttachInput(pInput);
	D3DHooker::Hook(Initialization);
}
