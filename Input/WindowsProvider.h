#pragma once
#include "../Header.h"

using namespace CEGUI;

class WindowsProvider : public NativeClipboardProvider
{
	static string GetClipboardText();
	static bool SetClipboardText(string str);

public:
	WindowsProvider();
	~WindowsProvider();

	//Load data to clipboard
	void sendToClipboard(const String& mimeType, void* buffer, size_t size) override;

	//Given data from clipboard
	void retrieveFromClipboard(String& mimeType, void*& buffer, size_t& size) override;
};

