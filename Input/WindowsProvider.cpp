#include "WindowsProvider.h"

string WindowsProvider::GetClipboardText()
{
	if (!OpenClipboard(NULL)) { return ""; }

	HANDLE hData = GetClipboardData(CF_TEXT);

	if (hData == NULL)
	{
		CloseClipboard();
		return "";
	}

	char* buffer = (char*)GlobalLock(hData);
	string str(buffer);
	GlobalUnlock(hData);
	CloseClipboard();
	return str;
}

bool WindowsProvider::SetClipboardText(string str)
{
	if (!OpenClipboard(NULL)) { return false; }

	EmptyClipboard();
	// Create a buffer to hold the string
	size_t iDataSize = str.size() + 1;
	HGLOBAL clipbuffer = GlobalAlloc(GMEM_MOVEABLE, iDataSize);
	// Copy the string into the buffer
	char* buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, str.c_str());
	GlobalUnlock(clipbuffer);
	// Place it on the clipboard
	SetClipboardData(CF_TEXT, clipbuffer);
	CloseClipboard();
	return true;
}

WindowsProvider::WindowsProvider()
{
}

WindowsProvider::~WindowsProvider()
{
}

void WindowsProvider::sendToClipboard(const String& mimeType, void* buffer, size_t size)
{
	string tmp = static_cast<char*>(buffer);
	tmp.resize(size);
	SetClipboardText(tmp);
}

void WindowsProvider::retrieveFromClipboard(String& mimeType, void*& buffer, size_t& size)
{
	string tmp = GetClipboardText();
	size = tmp.size();
	buffer = new char[size];
	memcpy(buffer, tmp.c_str(), size);
}
