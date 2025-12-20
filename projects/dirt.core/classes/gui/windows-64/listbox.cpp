/***********************************************
*
* File: listbox.cpp
*
* Purpose: listbox.hpp definitions
*
* Project: jujubee
*
************************************************/

#include NAMES_INCLUDE
#include LISTBOX_INCLUDE_PATH


juju::listbox::listbox(listbox_description lbd)
	:m_lbd(lbd)
{

}

void juju::listbox::action(listbox_commands command) {
	m_command = command;
	m_lbd.listbox_caller(command);
}

juju::juju_codes juju::listbox::create() {
	
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
	
	
	m_lb_handle = CreateWindow(
		m_lbd.class_name.c_str(),
		m_lbd.window_name.c_str(),
		m_lbd.style_flags,
		m_lbd.xPos,
		m_lbd.yPos,
		m_lbd.width,
		m_lbd.height,
		m_lbd.window,
		m_lbd.menu,
		m_lbd.hinst,
		m_lbd.lpParam
	);

	if (m_lb_handle == nullptr) {
		return juju::juju_codes::hwnd_fail;
	}

	return juju_codes::success;
}

juju::juju_codes juju::listbox::add_string(const string& list_message) {
	LRESULT result = SendMessage(m_lb_handle, LB_ADDSTRING, 0, (LPARAM)list_message.c_str());
	if (result == LB_ERR) {
		return juju::juju_codes::lb_add_string_fail;
	}
	return juju::juju_codes::success;
}

juju::string juju::listbox::get_selection(juju_codes* code) {
	int index = SendMessage(m_lb_handle, LB_GETCURSEL, 0, 0);
	std::vector<character> display_line(LB_MAX_STRING);
	display_line[LB_MAX_STRING-1] = ROS('\0'); // null terminate
	if (index != LB_ERR) {
		SendMessage(m_lb_handle, LB_GETTEXT, index, (LPARAM)display_line.data());
		*code = juju_codes::success;
	}
	else {
		*code = juju::juju_codes::lb_get_selection_fail;
	}
	return string(display_line.data());
}
