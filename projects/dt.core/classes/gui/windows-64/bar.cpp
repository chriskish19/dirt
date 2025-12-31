/**********************************************************/
//
// File: bar.hpp
//
// Purpose: progress bar in win32 window definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_BAR_INCLUDE_PATH

core::gui::bar::bar(bar_description bd)
	:m_bd(bd){}

void core::gui::bar::action(bar_state bs)
{
	m_b_state = bs;
	m_bd.bar_caller(bs);
}

core::codes core::gui::bar::create() {

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

	m_bar_handle = CreateWindow(
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

	if (m_bar_handle == nullptr) {
		return core::codes::hwnd_fail;
	}

	return core::codes::success;
}
