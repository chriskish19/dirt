/***********************************************
*
* File: textbox.cpp
*
* Purpose: textbox.hpp definitions
*
* Project: jujubee
*
************************************************/


#include NAMES_INCLUDE
#include TEXTBOX_INCLUDE_PATH


juju::textbox::textbox(textbox_description td)
	:m_td(td){}

juju::juju_codes juju::textbox::create() {
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

	m_textbox_handle = CreateWindow(
		m_td.class_name.c_str(),
		m_td.window_name.c_str(),
		m_td.style_flags,
		m_td.xPos,
		m_td.yPos,
		m_td.width,
		m_td.height,
		m_td.window,
		m_td.menu,
		m_td.hinst,
		m_td.lpParam
	);

	if (m_textbox_handle == nullptr) {
		return juju_codes::hwnd_fail;
	}

	return juju_codes::success;
}

void juju::textbox::action(textbox_commands command) {
	m_command = command;
	m_td.textbox_caller(command);
}
