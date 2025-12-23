#include "main_entry.hpp"

/**********************************************************/
//
// File: main_entry.cpp
//
// Purpose: main_entry.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_MAIN_ENTRY_INCLUDE_PATH



core::backend::backend(const std::vector<arg_entry>& v)
    :process(v){}

core::codes core::backend::run(std::stop_token token)
{
    {
        /*
            copies files from found in copy entries and sets up necessary objects for watching directories
        */
        core::codes code = process_entry();
        if (code != core::codes::success) {						// if there is an error exit the program and return the error code
            auto error_code = api::match_code(code);			// returns a code_pkg with a string message. See "CORE_CODES_INCLUDE_PATH".
            api::output_em(error_code);						    // send a message out (could be terminal or gui)
            return code;						                // return error code
        }
    }

    {
        /*
            Main directory watching loop, monitors directories for changes and reports the changes to
            another loop which processes all the entries.
        */
        core::codes code = watch(token);
        if (code != core::codes::success) {						// if there is an error exit the program and return the error code
            auto error_code = api::match_code(code);			// returns a code_pkg with a string message. See "CORE_CODES_INCLUDE_PATH".
            api::output_em(error_code);						    // send a message out (could be terminal or gui)
            return code;						                // return error code
        }
    }

    // if we reach this point with no errors its a successful run
    // See the header "CORE_ARGS_INCLUDE_PATH" for actual code int value.
    return core::codes::success;
}

void core::terminal_process_commands(std::shared_ptr<commands_info> ci)
{
	switch (ci->command()) {
	case commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<error>(ci);
		if (error_info != nullptr) {
			std::cout << error_info->code.m_s_code << '\n';
			std::cout << error_info->location << '\n';
			std::cout << error_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<fs_exception>(ci);
		if (exception_info != nullptr) {
			std::cout << exception_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<message>(ci);
		if (message_info != nullptr) {
			std::cout << message_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<time>(ci);
		if (time_info != nullptr) {
			std::cout << time_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<unknown_exception>(ci);
		if (unknown_exception_info != nullptr) {
			std::cout << unknown_exception_info->code.m_s_code << '\n';
			std::cout << unknown_exception_info->location << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<file_copy>(ci);
		if (file_copy_info != nullptr) {
			std::cout << file_copy_info->code.m_s_code << '\n';
			std::cout << file_copy_info->text << '\n';
			std::cout << file_copy_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<file_delete>(ci);
		if (file_delete_info != nullptr) {
			std::cout << file_delete_info->code.m_s_code << '\n';
			std::cout << file_delete_info->text << '\n';
			std::cout << file_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<directory_delete>(ci);
		if (directory_delete_info != nullptr) {
			std::cout << directory_delete_info->code.m_s_code << '\n';
			std::cout << directory_delete_info->text << '\n';
			std::cout << directory_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	default:
		break;
	}
}

void core::test_terminal_entry::go() {
	std::jthread worker_t(&test_terminal_entry::work, this, FLOOD);

	while (true) {
		// timer here, seconds to wait time
		std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));
		auto q = get_current_queue();
		while (q.empty() == false) {
			auto command = q.front();
			terminal_process_commands(command);
			q.pop();
		}
	}
}

void core::terminal_entry::go() {
	std::jthread backend_run_t([this](std::stop_token token) { m_be->run(token); });

	while (true) {
		// timer here, seconds to wait time
		std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));
		auto q = m_be->get_current_queue();
		while (q.empty() == false) {
			auto& command = q.front();
			terminal_process_commands(command);
			q.pop();
		}
	}
}

core::codes core::terminal_entry::init() {
	{
		codes code;
		m_v = api::cmd_line(m_argc, m_argv, &code);
		if (code != codes::success) {
			auto error_code = api::match_code(code);
			std::cout << error_code.m_s_code << '\n';
			return code;
		}
	}

	{
		codes code = api::validate(m_v);
		if (code != codes::success) {
			auto error_code = api::match_code(code);
			std::cout << error_code.m_s_code << '\n';
			return code;
		}
	}

	m_be = std::make_unique<backend>(m_v);
	return core::codes::success;
}

void core::gui_with_terminal_entry::process_commands(std::shared_ptr<commands_info> ci)
{
	switch (ci->command()) {
	case commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<error>(ci);
		if (error_info != nullptr) {
			std::cout << error_info->code.m_s_code << '\n';
			std::cout << error_info->location << '\n';
			std::cout << error_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<fs_exception>(ci);
		if (exception_info != nullptr) {
			std::cout << exception_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<message>(ci);
		if (message_info != nullptr) {
			std::cout << message_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<time>(ci);
		if (time_info != nullptr) {
			std::cout << time_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<unknown_exception>(ci);
		if (unknown_exception_info != nullptr) {
			std::cout << unknown_exception_info->code.m_s_code << '\n';
			std::cout << unknown_exception_info->location << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<file_copy>(ci);
		if (file_copy_info != nullptr) {
			std::cout << file_copy_info->code.m_s_code << '\n';
			std::cout << file_copy_info->text << '\n';
			std::cout << file_copy_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<file_delete>(ci);
		if (file_delete_info != nullptr) {
			std::cout << file_delete_info->code.m_s_code << '\n';
			std::cout << file_delete_info->text << '\n';
			std::cout << file_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<directory_delete>(ci);
		if (directory_delete_info != nullptr) {
			std::cout << directory_delete_info->code.m_s_code << '\n';
			std::cout << directory_delete_info->text << '\n';
			std::cout << directory_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.m_s_code << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	default:
		break;
	}
}

void core::gui_entry::gui_process_commands(std::shared_ptr<commands_info> ci)
{
	switch (ci->command())
	{
	case commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<error>(ci);
		if (error_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(error_info->code.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(error_info->location));
			logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(error_info->time_tp)));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<fs_exception>(ci);
		if (exception_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(exception_info->text));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<message>(ci);
		if (message_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(message_info->text));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<time>(ci);
		if (time_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(time_info->text));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<unknown_exception>(ci);
		if (unknown_exception_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(unknown_exception_info->code.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(unknown_exception_info->location));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<file_copy>(ci);
		if (file_copy_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(file_copy_info->code.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(file_copy_info->text));
			logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(file_copy_info->time_tp)));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<file_delete>(ci);
		if (file_delete_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(file_delete_info->code.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(file_delete_info->text));
			logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(file_delete_info->time_tp)));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<directory_delete>(ci);
		if (directory_delete_info != nullptr)
		{
			logger::glb_sl->log_message(api::to_wide_string(directory_delete_info->code.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(directory_delete_info->text));
			logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(directory_delete_info->time_tp)));
		}
		else
		{
			logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.m_s_code));
			logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	default:
		break;
	}
}
