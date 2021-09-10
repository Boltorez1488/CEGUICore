#include "InputManager.h"
#include "Keys.h"

DWORD InputManager::VkToScan(DWORD code)
{
	switch(code)
	{
		VK_BACK
		VK_TAB
		VK_RETURN
		VK_SHIFT
		VK_CONTROL
		VK_MENU
		VK_PAUSE
		VK_CAPITAL
		VK_ESCAPE
		VK_SPACE
		VK_PRIOR
		VK_NEXT
		VK_END
		VK_HOME
		VK_LEFT
		VK_UP
		VK_RIGHT
		VK_DOWN
		VK_INSERT
		VK_DELETE
		VK_0
		VK_1
		VK_2
		VK_3
		VK_4
		VK_5
		VK_6
		VK_7
		VK_8
		VK_9
		VK_A
		VK_B
		VK_C
		VK_D
		VK_E
		VK_F
		VK_G
		VK_H
		VK_I
		VK_J
		VK_K
		VK_L
		VK_M
		VK_N
		VK_O
		VK_P
		VK_Q
		VK_R
		VK_S
		VK_T
		VK_U
		VK_V
		VK_W
		VK_X
		VK_Y
		VK_Z
		VK_LWIN
		VK_RWIN
		VK_APPS
		VK_SLEEP
		VK_NUMPAD0
		VK_NUMPAD1
		VK_NUMPAD2
		VK_NUMPAD3
		VK_NUMPAD4
		VK_NUMPAD5
		VK_NUMPAD6
		VK_NUMPAD7
		VK_NUMPAD8
		VK_NUMPAD9
		VK_MULTIPLY
		VK_ADD
		VK_SUBTRACT
		VK_DECIMAL
		VK_DIVIDE
		VK_F1
		VK_F2
		VK_F3
		VK_F4
		VK_F5
		VK_F6
		VK_F7
		VK_F8
		VK_F9
		VK_F10
		VK_F11
		VK_F12
		VK_F13
		VK_F14
		VK_F15
		VK_NUMLOCK
		VK_SCROLL
		VK_PRINTSCREEN
		VK_LSHIFT
		VK_RSHIFT
		VK_LCONTROL
		VK_RCONTROL
		VK_BROWSER_BACK
		VK_BROWSER_FORWARD
		VK_BROWSER_REFRESH
		VK_BROWSER_STOP
		VK_BROWSER_SEARCH
		VK_BROWSER_FAVORITES
		VK_BROWSER_HOME
		VK_VOLUME_MUTE
		VK_VOLUME_DOWN
		VK_VOLUME_UP
		VK_MEDIA_NEXT_TRACK
		VK_MEDIA_PREV_TRACK
		VK_MEDIA_STOP
		VK_MEDIA_PLAY_PAUSE
		VK_LAUNCH_MAIL
		VK_LAUNCH_MEDIA_SELECT
		VK_OEM_1
		VK_OEM_PLUS
		VK_OEM_COMMA
		VK_OEM_MINUS
		VK_OEM_PERIOD
		VK_OEM_2
		VK_OEM_3
		VK_OEM_4
		VK_OEM_5
		VK_OEM_6
		VK_OEM_7
		VK_OEM_102
	default: return 0;
	}
}

void InputManager::InitClipboard()
{
	if(!cb)
	{
		cb = new Clipboard;
		win = new WindowsProvider;
		cb->setNativeProvider(win);
	}
}

bool keystate[256] = { false };

BOOL InputManager::RedirectClipboard(const MSG msg) const
{
	//auto key = MapVirtualKeyW(msg.wParam, 0);
	auto key = VkToScan(msg.wParam);
	switch(msg.message)
	{
	case WM_KEYDOWN:
		keystate[key] = true;
		break;
	case WM_KEYUP:
		keystate[key] = false;
		break;
	default:
		break;
	}
	if (keystate[Key::LeftControl] || keystate[Key::RightControl])
	{
		if(keystate[Key::X])
		{
			d_guiContext->getRootWindow()->getActiveChild()->performCut(*cb);
			return true;
		}
		if (keystate[Key::C])
		{
			d_guiContext->getRootWindow()->getActiveChild()->performCopy(*cb);
			return true;
		}
		if (keystate[Key::V])
		{
			d_guiContext->getRootWindow()->getActiveChild()->performPaste(*cb);
			return true;
		}
		if (keystate[Key::A])
		{
			//Editbox a; a.setSelection(0, a.getText().size())
			auto child = d_guiContext->getRootWindow()->getActiveChild();
			if (child->getType().find("/Editbox") != -1)
			{
				auto box = static_cast<Editbox*>(child);
				box->setSelection(0, box->getText().size());
				box->setCaretIndex(box->getText().size());
				return true;
			}
			if (child->getType().find("/MultiLineEditbox") != -1)
			{
				auto box = static_cast<MultiLineEditbox*>(child);
				box->setSelection(0, box->getText().size());
				box->setCaretIndex(box->getText().size());
				return true;
			}
		}
	}
	/*if (keystate[Key::LeftShift] || keystate[Key::RightShift])
	{
		if (keystate[Key::ArrowLeft] || keystate[Key::Numpad4])
		{
			auto child = d_guiContext->getRootWindow()->getActiveChild();
			if (child->getType().find("/Editbox") != -1)
			{
				auto box = static_cast<Editbox*>(child);
				auto start = box->getSelectionStartIndex(), end = box->getSelectionEndIndex(), caret = box->getCaretIndex();
				if (caret != 0)
					if (box->getSelectionLength() == 0)
						box->setSelection(caret - 1, caret);
					else if (start < caret)
						box->setSelection(start, caret - 1);
					else
						box->setSelection(caret - 1, end);
				if (box->getCaretIndex() < 2)
				{
					box->setCaretIndex(0);
					return true;
				}
				box->setCaretIndex(box->getCaretIndex() - 1);
				return true;
			}
			if (child->getType().find("/MultiLineEditbox") != -1)
			{
				auto box = static_cast<MultiLineEditbox*>(child);
				auto start = box->getSelectionStartIndex(), end = box->getSelectionEndIndex(), caret = box->getCaretIndex();
				if (caret != 0)
					if (box->getSelectionLength() == 0)
						box->setSelection(caret - 1, caret);
					else if (start < caret)
						box->setSelection(start, caret - 1);
					else
						box->setSelection(caret - 1, end);
				if (box->getCaretIndex() < 2)
				{
					box->setCaretIndex(0);
					return true;
				}
				box->setCaretIndex(box->getCaretIndex() - 1);
				return true;
			}
		}
		if (keystate[Key::ArrowRight] || keystate[Key::Numpad6])
		{
			auto child = d_guiContext->getRootWindow()->getActiveChild();
			if (child->getType().find("/Editbox") != -1)
			{
				auto box = static_cast<Editbox*>(child);
				auto start = box->getSelectionStartIndex(), end = box->getSelectionEndIndex(), caret = box->getCaretIndex();
				if (caret < box->getText().size())
					if (box->getSelectionLength() == 0)
						box->setSelection(caret, caret + 1);
					else if (end > caret)
						box->setSelection(caret + 1, end);
					else
						box->setSelection(start, caret + 1);
				if (caret > box->getText().size() - 1)
				{
					box->setCaretIndex(box->getText().size());
					return true;
				}
				box->setCaretIndex(box->getCaretIndex() + 1);
				return true;
			}
			if (child->getType().find("/MultiLineEditbox") != -1)
			{
				auto box = static_cast<MultiLineEditbox*>(child);
				auto start = box->getSelectionStartIndex(), end = box->getSelectionEndIndex(), caret = box->getCaretIndex();
				if (caret < box->getText().size())
					if (box->getSelectionLength() == 0)
						box->setSelection(caret, caret + 1);
					else if (end > caret)
						box->setSelection(caret + 1, end);
					else
						box->setSelection(start, caret + 1);
				if (caret > box->getText().size() - 1)
				{
					box->setCaretIndex(box->getText().size());
					return true;
				}
				box->setCaretIndex(box->getCaretIndex() + 1);
				return true;
			}
		}
	}*/
	if (keystate[Key::Return] || keystate[Key::NumpadEnter])
	{
		auto child = d_guiContext->getRootWindow()->getActiveChild();
		if (child->getType().find("/Editbox") != -1)
		{
			child->deactivate();
			return true;
		}
		if((keystate[Key::LeftShift] || keystate[Key::RightShift]) && child->getType().find("/MultiLineEditbox") != -1)
		{
			child->deactivate();
			return true;
		}
	}
	//if (keystate[Key::ArrowLeft])
	//{
	//	auto child = d_guiContext->getRootWindow()->getActiveChild();
	//	if (child->getType().find("/Editbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		if (box->getSelectionLength() != 0)
	//			box->setSelection(0, 0);
	//		if(box->getCaretIndex() < 2)
	//		{
	//			box->setCaretIndex(0);
	//			return true;
	//		}
	//		box->setCaretIndex(box->getCaretIndex() - 1);
	//		return true;
	//	}
	//	if (child->getType().find("/MultiLineEditbox") != -1)
	//	{
	//		auto box = static_cast<MultiLineEditbox*>(child);
	//		if (box->getSelectionLength() != 0)
	//			box->setSelection(0, 0);
	//		if (box->getCaretIndex() < 2)
	//		{
	//			box->setCaretIndex(0);
	//			return true;
	//		}
	//		box->setCaretIndex(box->getCaretIndex() - 1);
	//		return true;
	//	}
	//}
	//if (keystate[Key::ArrowRight])
	//{
	//	auto child = d_guiContext->getRootWindow()->getActiveChild();
	//	if (child->getType().find("/Editbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		if (box->getSelectionLength() != 0)
	//			box->setSelection(0, 0);
	//		if (box->getCaretIndex() > box->getText().size() - 1)
	//		{
	//			box->setCaretIndex(box->getText().size());
	//			return true;
	//		}
	//		box->setCaretIndex(box->getCaretIndex() + 1);
	//		return true;
	//	}
	//	if (child->getType().find("/MultiLineEditbox") != -1)
	//	{
	//		auto box = static_cast<MultiLineEditbox*>(child);
	//		if (box->getSelectionLength() != 0)
	//			box->setSelection(0, 0);
	//		if (box->getCaretIndex() > box->getText().size() - 1)
	//		{
	//			box->setCaretIndex(box->getText().size());
	//			return true;
	//		}
	//		box->setCaretIndex(box->getCaretIndex() + 1);
	//		return true;
	//	}
	//}
	//if (keystate[Key::End])
	//{
	//	auto child = d_guiContext->getRootWindow()->getActiveChild();
	//	if (child->getType().find("/Editbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		box->setCaretIndex(box->getText().size());
	//		return true;
	//	}
	//	if (child->getType().find("/MultiLineEditbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		box->setCaretIndex(box->getText().size());
	//		return true;
	//	}
	//}
	//if (keystate[Key::Home])
	//{
	//	auto child = d_guiContext->getRootWindow()->getActiveChild();
	//	if (child->getType().find("/Editbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		box->setCaretIndex(0);
	//		return true;
	//	}
	//	if (child->getType().find("/MultiLineEditbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		box->setCaretIndex(0);
	//		return true;
	//	}
	//}
	//if(keystate[Key::Delete])
	//{
	//	auto child = d_guiContext->getRootWindow()->getActiveChild();
	//	if (child->getType().find("/Editbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		if(box->getSelectionLength() != 0)
	//		{
	//			box->setText(box->getText().substr(box->getSelectionStartIndex(), box->getSelectionLength()));
	//			return true;
	//		}
	//		
	//		return true;
	//	}
	//	if (child->getType().find("/MultiLineEditbox") != -1)
	//	{
	//		auto box = static_cast<Editbox*>(child);
	//		box->setCaretIndex(0);
	//		return true;
	//	}
	//}
	return false;
}

BOOL InputManager::MouseChecker(const MSG msg) const
{
	switch(msg.message)
	{
	case WM_MOUSEMOVE:
		d_guiContext->injectMousePosition(static_cast<float>(LOWORD(msg.lParam)), static_cast<float>(HIWORD(msg.lParam)));
		return CursorCheck();

	case WM_LBUTTONDOWN:
		d_guiContext->injectMouseButtonDown(LeftButton);
		return CursorCheck();

	case WM_LBUTTONUP:
		d_guiContext->injectMouseButtonUp(LeftButton);
		return CursorCheck();

	case WM_RBUTTONDOWN:
		d_guiContext->injectMouseButtonDown(RightButton);
		return CursorCheck();

	case WM_RBUTTONUP:
		d_guiContext->injectMouseButtonUp(RightButton);
		return CursorCheck();

	case WM_MBUTTONDOWN:
		d_guiContext->injectMouseButtonDown(MiddleButton);
		return CursorCheck();

	case WM_MBUTTONUP:
		d_guiContext->injectMouseButtonUp(MiddleButton);
		return CursorCheck();

	default:
		return false;
	}
}

BOOL InputManager::CursorCheck() const
{
	if (d_guiContext->getRootWindow() && d_guiContext->getRootWindow()->isVisible())
	{
		auto child = d_guiContext->getRootWindow()->getTargetChildAtPosition(d_guiContext->getMouseCursor().getPosition());
		if (!child)
			return false;
		/*if (child->getType().find("/Editbox") != -1)
		{
			ShowCursor(false);
			return true;
		}
		if (child->getType().find("/MultiLineEditbox") != -1)
		{
			ShowCursor(false);
			return true;
		}
		ShowCursor(true);*/
		return true;
	}
	return false;
}

InputManager::InputManager()
{
	d_guiContext = NULL;
	win = NULL;
	cb = NULL;
	InitClipboard();
}

InputManager::~InputManager()
{
}

void InputManager::SetContext(GUIContext* context)
{
	d_guiContext = context;
}

BOOL InputManager::TranslateMessage(const MSG* lpMsg) const
{
	switch (lpMsg->message)
	{
	case WM_KEYDOWN:
		if (RedirectClipboard(*lpMsg))
			return true;
		return d_guiContext->injectKeyDown(static_cast<Key::Scan>(VkToScan(lpMsg->wParam)));

	case WM_KEYUP:
		if (RedirectClipboard(*lpMsg))
			return true;
		return d_guiContext->injectKeyUp(static_cast<Key::Scan>(VkToScan(lpMsg->wParam)));

	case WM_CHAR:
		return d_guiContext->injectChar(static_cast<utf32>(lpMsg->wParam));

	case WM_MOUSEMOVE:
		return MouseChecker(*lpMsg);

	case WM_LBUTTONDOWN:
		return MouseChecker(*lpMsg);

	case WM_LBUTTONUP:
		return MouseChecker(*lpMsg);

	case WM_RBUTTONDOWN:
		return MouseChecker(*lpMsg);

	case WM_RBUTTONUP:
		return MouseChecker(*lpMsg);

	case WM_MBUTTONDOWN:
		return MouseChecker(*lpMsg);

	case WM_MBUTTONUP:
		return MouseChecker(*lpMsg);

	case 0x020A: // WM_MOUSEWHEEL:
		return d_guiContext->injectMouseWheelChange(static_cast<float>(static_cast<short>(HIWORD(lpMsg->wParam))) / static_cast<float>(120));
	
	default: 
		return false;
	}
}
