/**********************************************************/
//
// File: dw.hpp (dirt window)
//
// Purpose: create a win32 window for dirt
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_WIN32_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH



namespace core{
	class foundation {
	public:
		foundation() = default;
	protected:
		virtual LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
		virtual codes window_settings() = 0;
		virtual codes create_window() = 0;
		virtual codes message_pump() = 0;

		HWND m_window_handle = nullptr;
		HINSTANCE m_hinst = GetModuleHandle(NULL);
		string m_c_name = L"foundation";
		string m_title = L"plain window";
		WNDCLASS m_wc = {};
	};
	
	
	class starter :public foundation {
	public:
		starter() = default;
		starter(const string& class_name);
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	protected:
		LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		codes window_settings() override;
		codes create_window() override;
		codes message_pump() override;
	};

	class window :starter{
	public:
		window();

		void go();

	protected:
		LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		codes add_dynamic_menu(HWND window_handle);

		codes set_icon(HWND hwnd, const fs::path& icon_path);
	};
}

