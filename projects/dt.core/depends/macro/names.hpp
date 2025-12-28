/**********************************************************/
//
// File: names.hpp
//
// Purpose: easy file moves/ name changes
//
// Project: dt.core
//
/**********************************************************/


/* 
	Note: there is a macro defined in cmake named "CORE_NAMES_INCLUDE" which is equal to 
	the file path of this header file. Include this header first in all subsequent header files.
*/

#pragma once

/*
	dependencies : contains all needed external include headers
*/
#define CORE_STL_PRIVATE_INCLUDE_PATH			CORE_stl_includes_hpp				// the entire stl library
#define CORE_STL_INCLUDE_PATH					CORE_stl_hpp						// specific version include stl library
#define CORE_WIN32_INCLUDE_PATH					CORE_windows_dps_hpp				// windows headers

/*
	codes : contains common enums and error codes, also simple common objects
*/
#define CORE_CODES_INCLUDE_PATH					CORE_codes_hpp						// error codes
#define CORE_ARGS_INCLUDE_PATH					CORE_args_hpp						// cmdline arguments

/*
	api : header that contains only functions used throughout dirt.core
*/
#define CORE_API_INCLUDE_PATH					CORE_dt_api_hpp						// helper functions

/*
	classes : big objects needed for dirt.cores functionality
*/
#define CORE_MAIN_ENTRY_INCLUDE_PATH			CORE_main_entry_hpp					// main singleton class
#define CORE_PROCESSOR_INCLUDE_PATH				CORE_processor_hpp					// processes files
#define CORE_LOGGER_INCLUDE_PATH				CORE_logger_hpp						// logger queue
#define CORE_TOUT_INCLUDE_PATH					CORE_output_hpp						// outputing
#define CORE_WINDOW_INCLUDE_PATH				CORE_dt_window_hpp					// win32 window
#define CORE_UTILITY_INCLUDE_PATH				CORE_utility_hpp					// utilities

// windows 64 gui
#define CORE_BUTTON_INCLUDE_PATH				CORE_button_hpp						// button class
#define CORE_LISTBOX_INCLUDE_PATH				CORE_listbox_hpp					// list box class
#define CORE_LABEL_INCLUDE_PATH					CORE_label_hpp						// label class
#define CORE_TEXTBOX_INCLUDE_PATH				CORE_textbox_hpp					// textbox class
#define CORE_UI_INCLUDE_PATH					CORE_ui_hpp							// main ui

// logger
#define CORE_BASE_INCLUDE_PATH					CORE_base_hpp						// base logger
#define CORE_FILELOG_INCLUDE_PATH				CORE_file_hpp						// file logger
#define CORE_MQSYS_INCLUDE_PATH					CORE_m_q_sys_hpp					// message queue system
#define CORE_SYSINIT_INCLUDE_PATH				CORE_system_init_hpp				// main global object for logging
#define CORE_WL_INCLUDE_PATH					CORE_wl_hpp							// window logger

/*
	simulation : everything needed for testing without mashing my ssd
*/
#define CORE_SIM_API_INCLUDE_PATH				CORE_sim_api_hpp					// simulation api
#define CORE_SIM_INCLUDE_PATH					CORE_sim_hpp						// sim classes

/*
	macros : common pre-processor values used throughout dirt.core
*/
#define CORE_DEFINES_INCLUDE_PATH				CORE_defines_hpp					// macros