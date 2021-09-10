#pragma once
#include "../Header.h"
#include "WindowsProvider.h"

using namespace CEGUI;

class InputManager
{
	GUIContext* d_guiContext;
	Clipboard* cb;
	WindowsProvider* win;

	static DWORD VkToScan(DWORD code);

	void InitClipboard();
	BOOL RedirectClipboard(const MSG msg) const;
	BOOL MouseChecker(const MSG msg) const;

	BOOL CursorCheck() const;

public:
	explicit InputManager();
	~InputManager();

	void SetContext(GUIContext* context);
	BOOL TranslateMessage(const MSG *lpMsg) const;
};

