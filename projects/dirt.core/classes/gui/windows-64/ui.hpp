/***********************************************
* 
* File: ui.hpp
*
* Purpose: handle user interface in jujubee
*
* Project: jujubee
*
************************************************/

#pragma once
#include NAMES_INCLUDE
#include DEPENDENCIES_INCLUDE_PATH
#include BUTTON_INCLUDE_PATH
#include LISTBOX_INCLUDE_PATH
#include LABEL_INCLUDE_PATH
#include TEXTBOX_INCLUDE_PATH



namespace juju {
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
		label
	};

	
	class ui {
	public:
		ui() = default;
		ui(HWND window,HMENU menu,HINSTANCE hinst,LPVOID lp);
		

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
		juju::button_description m_launch_bd{
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

		juju::button_description m_refresh_bd{
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
}