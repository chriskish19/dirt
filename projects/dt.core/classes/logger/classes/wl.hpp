/**********************************************************/
//
// File: wl.hpp (window logger)
//
// Purpose: a window which displays system logs
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_CODES_INCLUDE_PATH
#include CORE_BASE_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH

namespace core {
	namespace logger {
		struct window_description {
			DWORD					dwExStyle;    // The extended window style of the window being created
			const_character_p		lpClassName;  // The class name of the window
			const_character_p		lpWindowName; // The name/title of the window
			DWORD					dwStyle;      // The style of the window
			int						X;            // The initial horizontal position of the window
			int						Y;            // The initial vertical position of the window
			int						nWidth;       // The width of the window
			int						nHeight;      // The height of the window
			HWND					hWndParent;   // Handle to the parent or owner window
			HMENU					hMenu;        // Handle to a menu, or child-window identifier
			HINSTANCE				hInstance;    // Handle to the instance of the module to be associated with the window
			LPVOID					lpParam;      // Pointer to additional application-defined data
		};

		HWND create_window(const window_description& wd);		// create a window from a window description
		codes register_window(const WNDCLASSEX& wc);			// register a window class

		class window {
		public:
			window() = default;
			static LRESULT CALLBACK s_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);				// static window procedure
			virtual codes load();																					// initialize the window
		protected:
			HINSTANCE m_module = GetModuleHandle(NULL);																// module handle
			HWND m_handle = nullptr;																				// window handle
			virtual LRESULT CALLBACK this_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);			// class member function window procedure
			window_description m_wd;																				// init in load()
			WNDCLASSEX m_wc;																						// init in load()
		};

		class classic_log_window : public window, public base {
		public:
			classic_log_window();
			~classic_log_window();
			codes load() override;
			void thread_go();
			codes wait_until_init();
		protected:
			LRESULT CALLBACK this_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
			int m_nol = LOGGER_LINES - 1;
			int m_yChar = LOGGER_FONT_SIZE + 2;
			int m_xChar = 0;
			int m_xClientMax = 0;
			int m_font_size = LOGGER_FONT_SIZE;
			int m_xUpper = 0;
			int m_yClient = 0;
			int m_xClient = 0;
			int m_xPos = 0;
			int m_yPos = 0;
			HFONT m_clw_font = nullptr;					// custom font
			std::atomic<bool> m_wait_b = false;
			std::condition_variable m_wait_cv;
		};
	}
}