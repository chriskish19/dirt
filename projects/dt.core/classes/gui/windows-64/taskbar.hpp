/**********************************************************/
//
// File: task_bar.hpp
//
// Purpose: handle taskbar creation in win32
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH

namespace core {
	namespace gui {
		enum class task_bar_state {
			none,
			rest
		};

		struct task_bar_description {
			string class_name = TOOLBARCLASSNAME;
			string window_name = ROS("#task_bar");
			DWORD style_flags = 0;
			std::size_t xPos = 0;
			std::size_t yPos = 0;
			std::size_t width = 0;
			std::size_t height = 0;
			HWND window = nullptr;
			HMENU menu = nullptr;
			HINSTANCE hinst = GetModuleHandle(NULL);
			LPVOID lpParam = nullptr;
			std::function<void(task_bar_state)> task_bar_caller = nullptr;
		};

		class task_bar {
		public:
			task_bar() = default;
			task_bar(task_bar_description bd);
			core::codes create();
			virtual void action(task_bar_state bs);
			task_bar_description get_bd() { return m_bd; }
			HWND get_task_bar_handle() { return m_task_bar_handle; }
			task_bar_state get_task_bar_state() { return m_b_state; }
		protected:
			task_bar_description m_bd = {};
			HWND m_task_bar_handle = nullptr;
			task_bar_state m_b_state = task_bar_state::rest;
		};
	}
}

/*

			TBBUTTON m_tbButtons[4] = {
				{ 0, static_cast<int>(window_ids::tbar_new),   TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"New"},
				{ 1, static_cast<int>(window_ids::tbar_open),  TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Open" },
				{ 2, static_cast<int>(window_ids::tbar_save),  TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Save" },
				{ 3, static_cast<int>(window_ids::tbar_copy),  TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Copy" }
			};

			const int IMAGELISTID = 0;
			const int ICON_WIDTH = 24;
			const int ICON_HEIGHT = 24;
			const int NUM_ICONS = 4;
			HBITMAP hBmpColor = (HBITMAP)LoadImage(
				NULL,
				DT_TOOLBAR_PATH,
				IMAGE_BITMAP,
				0, 0,
				LR_LOADFROMFILE);
			HBITMAP hBmpMask = NULL;
			HIMAGELIST hImageList = ImageList_Create(ICON_WIDTH, ICON_HEIGHT, ILC_COLOR24, NUM_ICONS, 0);
			int added = ImageList_Add(hImageList, hBmpColor, hBmpMask);
			SendMessage(m_tbar.get_task_bar_handle(), TB_SETIMAGELIST, (WPARAM)IMAGELISTID, (LPARAM)hImageList);
			SendMessage(m_tbar.get_task_bar_handle(), TB_LOADIMAGES,(WPARAM)IDB_STD_SMALL_COLOR,(LPARAM)HINST_COMMCTRL);
			SendMessage(m_tbar.get_task_bar_handle(), TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage(m_tbar.get_task_bar_handle(), TB_ADDBUTTONS, (WPARAM)NUM_ICONS, (LPARAM)&m_tbButtons);
			SendMessage(m_tbar.get_task_bar_handle(), TB_AUTOSIZE, 0, 0);
			DeleteObject(hBmpColor);

*/