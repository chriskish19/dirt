/***********************************************
* 
* File: button.hpp
*
* Purpose: handle buttons in win32 window.
*
* Project: jujubee
*
************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include DEPENDENCIES_INCLUDE_PATH
#include ERROR_INCLUDE_PATH
#include CODES_INCLUDE_PATH

namespace core {
	enum class button_state {
		rest = 0,
		pressed,
		released,
		holding
	};


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
	
	
	class button {
	public:
		button() = default;
		button(button_description bd);

		core::codes create();

		virtual void action(button_state bs);

		button_description get_bd() { return m_bd; }
		HWND get_button_handle() { return m_button_handle; }
		button_state get_button_state() { return m_b_state; }
	protected:
		button_description m_bd = {};
		HWND m_button_handle = nullptr;
		button_state m_b_state = button_state::rest;
	};
}