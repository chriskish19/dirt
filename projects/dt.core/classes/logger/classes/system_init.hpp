/***************************************
*  File: system_init.hpp (system initialize)
*
*  Purpose: initilizes the core
*			and provides functions for 
*			using it
*
*  Project: dt.core
* *************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_CODES_INCLUDE_PATH
#include CORE_WL_INCLUDE_PATH
#include CORE_MQSYS_INCLUDE_PATH


namespace core {
#if MANUAL_LOGGER
	extern LOGS_API core::classic_log_window* log_terminal;
	extern LOGS_API std::thread* lt_thread;

	// initialize the core system
	codes LOGS_API init_system_log();

	void LOGS_API log_message(const string& message);

	// exit the core system
	// make sure to call this if you call init_system_log()
	codes LOGS_API exit_system_log();
#else

    // use this class instead of functions
    class system_log_window : public classic_log_window{
    public:
        system_log_window();
        ~system_log_window();

        void log_message(const string& message);
        HWND get_window_handle() { return m_handle; }
    };

    // global system logger object
    extern std::unique_ptr<system_log_window> glb_sl;

#endif
}