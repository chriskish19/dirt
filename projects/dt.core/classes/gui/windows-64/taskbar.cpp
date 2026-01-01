/**********************************************************/
//
// File: taskbar.cpp
//
// Purpose: taskbar.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_TASKBAR_INCLUDE_PATH

core::gui::task_bar::task_bar(task_bar_description bd)
	:m_bd(bd)
{

}

void core::gui::task_bar::action(task_bar_state bs)
{
	m_b_state = bs;
	m_bd.task_bar_caller(bs);
}

core::codes core::gui::task_bar::create() {

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

	m_task_bar_handle = CreateWindow(
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

	if (m_task_bar_handle == nullptr) {
		return core::codes::hwnd_fail;
	}

	return core::codes::success;
}