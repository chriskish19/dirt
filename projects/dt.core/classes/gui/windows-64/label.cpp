#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#if UNDER_CONSTRUCTION

/***********************************************
*
* File: label.cpp
*
* Purpose: label.hpp definitions
*
* Project: dt.core
*
************************************************/


#include CORE_LABEL_INCLUDE_PATH

core::label::label(label_description ld)
	:m_ld(ld)
{

}

core::codes core::label::create() {
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

	m_label_handle = CreateWindow(
		m_ld.class_name.c_str(),
		m_ld.window_name.c_str(),
		m_ld.style_flags,
		m_ld.xPos,
		m_ld.yPos,
		m_ld.width,
		m_ld.height,
		m_ld.window,
		m_ld.menu,
		m_ld.hinst,
		m_ld.lpParam
	);

	if (m_label_handle == nullptr) {
		return codes::hwnd_fail;
	}

	return codes::success;
}

void core::label::action(label_commands command) {
	m_command = command;
	m_ld.label_caller(command);
}

#endif