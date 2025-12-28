/**********************************************************/
//
// File: button.cpp
//
// Purpose: defines a gui button in win32
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_BUTTON_INCLUDE_PATH

core::gui::button::button(button_description bd)
	:m_bd(bd){}

core::codes core::gui::button::create()
{
	/*
	
				HWND CreateWindowW(
			  [in, optional] LPCWSTR   lpClassName,
			  [in, optional] LPCWSTR   lpWindowName,
			  [in]           DWORD     dwStyle,
			  [in]           int       x,
			  [in]           int       y,
			  [in]           int       nWidth,
			  [in]           int       nHeight,
			  [in, optional] HWND      hWndParent,
			  [in, optional] HMENU     hMenu,
			  [in, optional] HINSTANCE hInstance,
			  [in, optional] LPVOID    lpParam
			);
	
	*/

	m_button_handle = CreateWindow(
		m_bd.class_name.c_str(),
		m_bd.window_name.c_str(),
		m_bd.style_flags,
		m_bd.xPos,
		m_bd.yPos,
		m_bd.width,
		m_bd.height,
		m_bd.window,
		m_bd.menu,
		m_bd.hinst,
		m_bd.lpParam
	);

	if (m_button_handle == nullptr) {
		return core::codes::hwnd_fail;
	}

	return core::codes::success;
}

void core::gui::button::action(button_state bs)
{
	m_b_state = bs;
	m_bd.button_caller(bs);
}

/*

switch (bs) {
		case button_state::rest:
		{
			return;
		}

		case button_state::pressed:
		{
			break;
		}

		case button_state::released:
		{
			break;
		}

		case button_state::holding:
		{
			break;
		}

		default:
			break;
	}

*/
