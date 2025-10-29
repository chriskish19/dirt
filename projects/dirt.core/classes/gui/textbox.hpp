/***********************************************
* 
* File: textbox.hpp
*
* Purpose: handle textbox's in a win32 window.
*
* Project: jujubee
*
************************************************/

#pragma once
#include NAMES_INCLUDE
#include DEPENDENCIES_INCLUDE_PATH
#include CODES_INCLUDE_PATH



namespace juju {
	enum class textbox_commands {
		none = 0,
		change = EN_CHANGE,					// Sent when the text changes(user input or programmatically).
		update = EN_UPDATE,					// Sent before the text changes(used for validation).
		set_focus = EN_SETFOCUS,			// Sent when the textbox gains focus.
		kill_focus = EN_KILLFOCUS,			// Sent when the textbox loses focus.
		error_out_of_memory = EN_ERRSPACE,	// Sent when the system runs out of memory for text storage.
		scroll_horizontal = EN_HSCROLL,		// Sent when the user scrolls horizontally.
		vertical_scroll = EN_VSCROLL,		// Sent when the user scrolls vertically.
		max_length = EN_MAXTEXT,			// Sent when text exceeds the maximum length.
		left_aligned = EN_ALIGN_LTR_EC,		// Sent when text is left - aligned.
		right_aligned = EN_ALIGN_RTL_EC		// Sent when text is right - aligned.
	};
	
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


	class textbox {
	public:
		textbox() = default;
		textbox(textbox_description td);

		juju_codes create();

		virtual void action(textbox_commands command);

		textbox_description get_td() { return m_td; }
		HWND get_textbox_handle() { return m_textbox_handle; }
		textbox_commands get_command() { return m_command; }
	protected:
		textbox_description m_td = {};
		HWND m_textbox_handle = nullptr;
		textbox_commands m_command = textbox_commands::none;
	};
}