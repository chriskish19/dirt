
/***********************************************
* 
* File: label.hpp
*
* Purpose: handle labels in a win32 window.
*
* Project: dt.core
*
************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH


namespace core {
	namespace gui {


		enum class label_commands {
			clicked = STN_CLICKED,		// Sent when the label is clicked.
			dbl_clicked = STN_DBLCLK,	//	Sent when the label is double - clicked.
			disable = STN_DISABLE,		//	Sent when the label is disabled.
			enable = STN_ENABLE,		//	Sent when the label is enabled.
			none = 4
		};

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


		class label {
		public:
			label() = default;
			label(label_description ld);

			codes create();

			virtual void action(label_commands command);

			label_description get_ld() { return m_ld; }
			HWND get_label_handle() { return m_label_handle; }
			label_commands get_command() { return m_command; }
		protected:
			label_description m_ld = {};
			HWND m_label_handle = nullptr;
			label_commands m_command = label_commands::none;
		};
	}
}

