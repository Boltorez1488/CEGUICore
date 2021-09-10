#pragma once
#define CEGUI_STATIC
#define NOMINMAX
#define MINIZIP_STATIC

#include <Windows.h>
#include <vector>

using namespace std;

#include <CEGUI/MinizipResourceProvider.h>
#include <CEGUI/Widgets/Editbox.h>
#include <CEGUI/widgets/MultiLineEditbox.h>
#include <CEGUI/Clipboard.h>
#include <CEGUI/InjectedInputReceiver.h>
#include <CEGUI/GUIContext.h>
#include <CEGUI/falagard/WidgetLookManager.h>
#include <CEGUI/XMLParser.h>
#include <CEGUI/AnimationManager.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/Font.h>
#include <CEGUI/ScriptModule.h>
#include <CEGUI/ResourceProvider.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/SchemeManager.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/System.h>
#include <CEGUI/Base.h>
#include "CEGUI/RendererModules/Direct3D9/Renderer.h"

#pragma comment(lib, "d3d9.lib")

#pragma comment(lib, "CEGUIBase-0_Static.lib")
#pragma comment(lib, "CEGUIExpatParser_Static.lib")
#pragma comment(lib, "CEGUISILLYImageCodec_Static.lib")
#pragma comment(lib, "CEGUICoreWindowRendererSet_Static.lib")
#pragma comment(lib, "glfw.lib")
#pragma comment(lib, "CEGUIDirect3D9Renderer-0_Static.lib")
#pragma comment(lib, "libexpat.lib")
#pragma comment(lib, "SILLY.lib")
#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "jpeg.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "glew.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "pcre.lib")
#pragma comment(lib, "minizip.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "cryptlib.lib")
#pragma comment(lib, "PocoFoundationmt.lib")
#pragma comment(lib, "PocoZipmt.lib")