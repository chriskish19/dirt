/**********************************************************/
//
// File: system_init.cpp (system initialize)
//
// Purpose: system_init.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_SYSINIT_INCLUDE_PATH

namespace core {
	namespace logger {
#if MANUAL_LOGGER
		core::logger::classic_log_window* log_terminal = nullptr;
		std::thread* lt_thread = nullptr;
#else
		// automatic system logger
		std::unique_ptr<system_log_window> glb_sl = nullptr;
#endif
	}
}

#if MANUAL_LOGGER
core::logger::logger::codes core::logger::logger::init_system_log()
{
	try {
		log_terminal = new core::logger::logger::classic_log_window;
		lt_thread = new std::thread(&core::logger::logger::classic_log_window::thread_go, log_terminal);
		log_terminal->wait_until_init();
	}
	catch (const core::logger::logger::le& e) {
		// core error
		CERROR << ROS("DESCRIPTION: ") << e.m_desc << '\n' <<
			ROS("WINOWS ERROR: ") << e.m_w32 << '\n' <<
			ROS("LOCATION: ") << e.m_loc;
		return codes::core_exception_caught;
	}
	catch (...) {
		CERROR << ROS("Unknown exception caught") << '\n' <<
			ROS("LOCATION: ") << gl();
		return codes::unknown_exception_caught;
	}
	return codes::success;
}

void core::logger::log_message(const string& message)
{
	log_terminal->send_log(message);
}

core::logger::codes core::logger::exit_system_log()
{
	log_terminal->send_log(ROS("waiting for log window to be closed..."));
	
	if(lt_thread->joinable())
		lt_thread->join();


	if (lt_thread != nullptr) {
		delete lt_thread;
		lt_thread = nullptr;
	}

	if (log_terminal != nullptr) {
		delete log_terminal;
		log_terminal = nullptr;
	}

	return codes::success;
}

#else

core::logger::system_log_window::system_log_window()
{
	try {
		core::logger::log_window::load();
	}
	catch (const core::le& e) {
		api::output_le(e);
	}
	catch (...) {
		std::string location = api::get_location();
		core::le e(unknown_exception_caught_pkg,location, api::get_last_error_w32());
		api::output_le(e);
	}
}

core::logger::system_log_window::~system_log_window(){}

void core::logger::system_log_window::log_message(const string& message)
{
	// get current log
	core::logger::log* log_p = nullptr;
	std::size_t index = 0;
	
	index = base::get_v_index();
	log_p = base::get_buffer()->at(index);
	*log_p->message = message;
	base::set_log(log_p);
	
	AppendText(m_p_main_ui->m_output_tb.get_textbox_handle(), message.c_str());
}

#endif
