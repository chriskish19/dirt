/***************************************
*  File: wl.hpp (window logger)
*
*  Purpose: a window which displays 
*			system logs
*
*  Project: dt.core
* *************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_CODES_INCLUDE_PATH
#include CORE_BASE_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH


namespace core {
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

	// create a window from a window description
	HWND create_window(const window_description& wd);

	// register a window class
	codes register_window(const WNDCLASSEX& wc);
	
	
	
	class l_window {
	public:
		l_window() = default;

		// static window procedure
		static LRESULT CALLBACK s_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// initialize the window
		virtual codes load();
	protected:
		// module handle
		HINSTANCE m_module = GetModuleHandle(NULL);

		// window handle
		HWND m_handle = nullptr;

		// class member function window procedure
		virtual LRESULT CALLBACK this_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// see above for definition
		// init in load()
		window_description m_wd;

		// window class
		// init in load()
		WNDCLASSEX m_wc;
	};
	
	



	// uses direct x to print the logs inside the window
	class dx_log_window : public l_window {
	public:
		dx_log_window() = default;

		HWND handle() { return m_handle; }
		UINT width();
		UINT height();
	protected:
		LRESULT CALLBACK this_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};

	// uses win32 api and classic window logging
	class classic_log_window : public core::l_window, public core::base {
	public:
		classic_log_window();
		~classic_log_window();
		codes load() override;
		void thread_go();
		codes wait_until_init();

		// length of time stamp
		const std::size_t m_ts_length = time_stamped(ROS("")).size();
	protected:
		LRESULT CALLBACK this_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		// scrolling
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

		// custom font
		HFONT m_clw_font = nullptr;

		// init
		std::atomic<bool> m_wait_b = false;
		std::condition_variable m_wait_cv;
	};
}