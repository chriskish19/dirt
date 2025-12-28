
/**********************************************************/
//
// File: dw.hpp (dt window)
//
// Purpose: create a win32 window 
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_UI_INCLUDE_PATH
#include CORE_TOUT_INCLUDE_PATH
#include CORE_SYSINIT_INCLUDE_PATH



namespace core{
	namespace window {

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
			codes message_pump() override;
		protected:
			LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
			codes window_settings() override;
			codes create_window() override;
		};

		class window : public starter, public core::backend::backend_message_queue {
		public:
			window();
		protected:
			LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
			codes add_dynamic_menu(HWND window_handle);
			codes set_icon(HWND hwnd, const fs::path& icon_path);
		};


		class dt_window : public window {
		public:
			dt_window() {
				m_p_main_ui = std::make_unique<core::gui::ui>(m_window_handle, nullptr, m_hinst, nullptr);

				if (m_p_main_ui == nullptr) {
					throw core::pointer_is_null_pkg;
				}

				m_p_main_ui->m_front_lb.add_string(ROS("Test string"));
			}

			HWND get_handle() { return m_window_handle; }
		protected:
			LRESULT CALLBACK ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
			std::unique_ptr<core::gui::ui> m_p_main_ui = nullptr;

			bool m_show_logger = false;
		};
	}
}
