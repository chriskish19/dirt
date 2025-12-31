/**********************************************************/
//
// File: bar.hpp
//
// Purpose: progress bar in win32 window.
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
		enum class bar_state {
			none,
			rest
		};

		struct bar_description {
			string class_name = PROGRESS_CLASS;
			string window_name = ROS("#bar");
			DWORD style_flags = 0;
			std::size_t xPos = 0;
			std::size_t yPos = 0;
			std::size_t width = 0;
			std::size_t height = 0;
			HWND window = nullptr;
			HMENU menu = nullptr;
			HINSTANCE hinst = GetModuleHandle(NULL);
			LPVOID lpParam = nullptr;
			std::function<void(bar_state)> bar_caller = nullptr;
		};

		class bar {
		public:
			bar() = default;
			bar(bar_description bd);
			core::codes create();
			virtual void action(bar_state bs);
			bar_description get_bd() { return m_bd; }
			HWND get_bar_handle() { return m_bar_handle; }
			bar_state get_bar_state() { return m_b_state; }
		protected:
			bar_description m_bd = {};
			HWND m_bar_handle = nullptr;
			bar_state m_b_state = bar_state::rest;
		};
	}
}