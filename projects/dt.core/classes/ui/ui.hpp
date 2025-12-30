/**********************************************************/
//
// File: ui.hpp
//
// Purpose: handle user interface in dt
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_STL_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH
#include CORE_BUTTON_INCLUDE_PATH
#include CORE_LISTBOX_INCLUDE_PATH
#include CORE_LABEL_INCLUDE_PATH
#include CORE_TEXTBOX_INCLUDE_PATH

namespace core {
	namespace gui {
		enum class ui_positions : std::size_t {
			// label
			label_x = 10,
			label_y = 10,
			label_w = 100,
			label_h = 20,

			// front listbox
			front_lb_x = 10,
			front_lb_y = 30,
			front_lb_w = 400,
			front_lb_h = 400,

			// refresh button
			refresh_b_x = 10,
			refresh_b_y = 420,
			refresh_b_w = 100,
			refresh_b_h = 50,

			// launch button
			launch_b_x = 310,
			launch_b_y = 420,
			launch_b_w = 100,
			launch_b_h = 50
		};

		enum class window_ids {
			console = 0,
			file,
			open,
			exit,
			help,
			b_front,
			b_refresh,
			lb_box,
			label,
			view,
			show_system_logger,
			cmdline,
		};

		class main_window_ui {
		public:
			main_window_ui() = default;
			main_window_ui(HWND window, HMENU menu, HINSTANCE hinst, LPVOID lp);
			button m_launch_b;
			void front_button_action(button_state bs);
			button m_refresh_b;
			void refresh_button_action(button_state bs);
			listbox m_front_lb;
			void front_listbox_action(listbox_commands lc);
			label m_lb_label;
			void lb_label_action(label_commands command);
		protected:
			/*

				struct button_description {
					string class_name = ROS("BUTTON");
					string window_name = ROS("#num");
					DWORD style_flags = 0;
					std::size_t xPos = 0;
					std::size_t yPos = 0;
					std::size_t width = 0;
					std::size_t height = 0;
					HWND window = nullptr;
					HMENU menu = nullptr;
					HINSTANCE hinst = GetModuleHandle(NULL);
					LPVOID lpParam = nullptr;
					std::function<void(button_state)> button_caller = nullptr;
				};

			*/
			core::gui::button_description m_launch_bd{
				ROS("BUTTON"),
				ROS("Launch"),
				BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
				static_cast<std::size_t>(ui_positions::launch_b_x),
				static_cast<std::size_t>(ui_positions::launch_b_y),
				static_cast<std::size_t>(ui_positions::launch_b_w),
				static_cast<std::size_t>(ui_positions::launch_b_h),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};

			core::gui::button_description m_refresh_bd{
				ROS("BUTTON"),
				ROS("Refresh"),
				BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
				static_cast<std::size_t>(ui_positions::refresh_b_x),
				static_cast<std::size_t>(ui_positions::refresh_b_y),
				static_cast<std::size_t>(ui_positions::refresh_b_w),
				static_cast<std::size_t>(ui_positions::refresh_b_h),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};

			/*

					struct listbox_description {
					string class_name = ROS("LISTBOX");
					string window_name = ROS("LB1");
					DWORD style_flags = 0;
					std::size_t xPos = 0;
					std::size_t yPos = 0;
					std::size_t width = 0;
					std::size_t height = 0;
					HWND window = nullptr;
					HMENU menu = nullptr;
					HINSTANCE hinst = GetModuleHandle(NULL);
					LPVOID lpParam = nullptr;
					std::function<void(listbox_commands)> listbox_caller = nullptr;
				};

			*/

			listbox_description m_front_lb_ld{
				ROS("LISTBOX"),
				ROS("LB_front"),
				WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD,
				static_cast<std::size_t>(ui_positions::front_lb_x),
				static_cast<std::size_t>(ui_positions::front_lb_y),
				static_cast<std::size_t>(ui_positions::front_lb_w),
				static_cast<std::size_t>(ui_positions::front_lb_h),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};

			/*

						struct label_description {
						string class_name = ROS("STATIC");
						string window_name = ROS("LABEL_TEXT");
						DWORD style_flags = 0;
						std::size_t xPos = 0;
						std::size_t yPos = 0;
						std::size_t width = 0;
						std::size_t height = 0;
						HWND window = nullptr;
						HMENU menu = nullptr;
						HINSTANCE hinst = GetModuleHandle(NULL);
						LPVOID lpParam = nullptr;
						std::function<void(label_commands)> label_caller = nullptr;
					};

			*/
			label_description m_lb_label_d{
				ROS("STATIC"),
				ROS("Program List"),
				WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY,
				static_cast<std::size_t>(ui_positions::label_x),
				static_cast<std::size_t>(ui_positions::label_y),
				static_cast<std::size_t>(ui_positions::label_w),
				static_cast<std::size_t>(ui_positions::label_h),
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};
		};

		class system_log_window_ui {
		public:
			system_log_window_ui() = default;
			system_log_window_ui(HWND window, HMENU menu, HINSTANCE hinst, LPVOID lp);

			textbox m_cmdinputbar_tb;
			void cmdinputbar_action(textbox_commands command);

			textbox m_output_tb;
			void output_action(textbox_commands command);
		protected:

			/*
				struct textbox_description {
					string class_name = ROS("EDIT");
					string window_name = ROS("TEXTBOX");
					DWORD style_flags = 0;
					std::size_t xPos = 0;
					std::size_t yPos = 0;
					std::size_t width = 0;
					std::size_t height = 0;
					HWND window = nullptr;
					HMENU menu = nullptr;
					HINSTANCE hinst = GetModuleHandle(NULL);
					LPVOID lpParam = nullptr;
					std::function<void(textbox_commands)> textbox_caller = nullptr;
				};
			
			*/


			textbox_description m_cmdinputbar_td{
				ROS("EDIT"),
				ROS("Type commands here... Press enter to execute"),
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_AUTOHSCROLL | WS_BORDER,
				0,
				0,
				0,
				50,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};

			textbox_description m_output_td{
				MSFTEDIT_CLASS,
				ROS(""),
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL | ES_MULTILINE,
				0,
				50,
				0,
				50,
				nullptr,
				nullptr,
				nullptr,
				nullptr,
				nullptr
			};
		};
	}
}

