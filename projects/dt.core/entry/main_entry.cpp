
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



core::main::backend::backend(const std::vector<arg_entry>& v)
    :process(v){}

core::codes core::main::backend::run()
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
        core::codes code = watch();
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

void core::main::terminal_process_commands(std::shared_ptr<core::backend::commands_info> ci)
{
	switch (ci->command()) {
	case core::backend::commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<core::backend::error>(ci);
		if (error_info != nullptr) {
			std::cout << error_info->code.message() << '\n';
			std::cout << error_info->location << '\n';
			std::cout << error_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<core::backend::fs_exception>(ci);
		if (exception_info != nullptr) {
			std::cout << exception_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<core::backend::message>(ci);
		if (message_info != nullptr) {
			std::cout << message_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<core::backend::time>(ci);
		if (time_info != nullptr) {
			std::cout << time_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case core::backend::commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<core::backend::unknown_exception>(ci);
		if (unknown_exception_info != nullptr) {
			std::cout << unknown_exception_info->code.message() << '\n';
			std::cout << unknown_exception_info->location << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case core::backend::commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<core::backend::file_copy>(ci);
		if (file_copy_info != nullptr) {
			std::cout << file_copy_info->code.message() << '\n';
			std::cout << file_copy_info->text << '\n';
			std::cout << file_copy_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case core::backend::commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<core::backend::file_delete>(ci);
		if (file_delete_info != nullptr) {
			std::cout << file_delete_info->code.message() << '\n';
			std::cout << file_delete_info->text << '\n';
			std::cout << file_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	case core::backend::commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<core::backend::directory_delete>(ci);
		if (directory_delete_info != nullptr) {
			std::cout << directory_delete_info->code.message() << '\n';
			std::cout << directory_delete_info->text << '\n';
			std::cout << directory_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		
		break;
	}

	default:
		break;
	}
}

void core::main::test_terminal_entry::go() {
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

void core::main::terminal_entry::go() {
	std::jthread backend_run_t([this] { m_be->run(); });

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

core::codes core::main::terminal_entry::init() {
	{
		codes code;
		m_v = api::cmd_line(m_argc, m_argv, &code);
		if (code != codes::success) {
			auto error_code = api::match_code(code);
			std::cout << error_code.message() << '\n';
			return code;
		}
	}

	{
		codes code = api::validate(m_v);
		if (code != codes::success) {
			auto error_code = api::match_code(code);
			std::cout << error_code.message() << '\n';
			return code;
		}
	}

	m_be = std::make_unique<backend>(m_v);
	return core::codes::success;
}

void core::main::gui_with_terminal_entry::process_commands(std::shared_ptr<core::backend::commands_info> ci)
{
	switch (ci->command()) {
	case core::backend::commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<core::backend::error>(ci);
		if (error_info != nullptr) {
			std::cout << error_info->code.message() << '\n';
			std::cout << error_info->location << '\n';
			std::cout << error_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<core::backend::fs_exception>(ci);
		if (exception_info != nullptr) {
			std::cout << exception_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<core::backend::message>(ci);
		if (message_info != nullptr) {
			std::cout << message_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<core::backend::time>(ci);
		if (time_info != nullptr) {
			std::cout << time_info->text << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<core::backend::unknown_exception>(ci);
		if (unknown_exception_info != nullptr) {
			std::cout << unknown_exception_info->code.message() << '\n';
			std::cout << unknown_exception_info->location << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<core::backend::file_copy>(ci);
		if (file_copy_info != nullptr) {
			std::cout << file_copy_info->code.message() << '\n';
			std::cout << file_copy_info->text << '\n';
			std::cout << file_copy_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<core::backend::file_delete>(ci);
		if (file_delete_info != nullptr) {
			std::cout << file_delete_info->code.message() << '\n';
			std::cout << file_delete_info->text << '\n';
			std::cout << file_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	case core::backend::commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<core::backend::directory_delete>(ci);
		if (directory_delete_info != nullptr) {
			std::cout << directory_delete_info->code.message() << '\n';
			std::cout << directory_delete_info->text << '\n';
			std::cout << directory_delete_info->time_tp << '\n';
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}

		break;
	}

	default:
		break;
	}
}

void core::main::gui_entry::gui_process_commands(std::shared_ptr<core::backend::commands_info> ci)
{
	switch (ci->command())
	{
	case core::backend::commands::update_progress_bar:
	{
		auto progress_bar_info = std::dynamic_pointer_cast<core::backend::progress_bar>(ci);
		if (progress_bar_info != nullptr) {
			switch (progress_bar_info->bar_number) {
			case core::backend::progress_bar::id::one:
				SendMessage(m_fe->get_ui_p()->m_progress_bar1.get_bar_handle(), PBM_SETPOS, progress_bar_info->progress, 0);
				break;
			case core::backend::progress_bar::id::two:
				SendMessage(m_fe->get_ui_p()->m_progress_bar2.get_bar_handle(), PBM_SETPOS, progress_bar_info->progress, 0);
				break;
			case core::backend::progress_bar::id::three:
				break;
			case core::backend::progress_bar::id::four:
				break;
			default:
				break;
			}
		}
		else {
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::error:
	{
		auto error_info = std::dynamic_pointer_cast<core::backend::error>(ci);
		if (error_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(error_info->code.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(error_info->location));
			core::logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(error_info->time_tp)));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::file_system_exception:
	{
		auto exception_info = std::dynamic_pointer_cast<core::backend::fs_exception>(ci);
		if (exception_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(exception_info->text));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::message:
	{
		auto message_info = std::dynamic_pointer_cast<core::backend::message>(ci);
		if (message_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(message_info->text));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::time_message:
	{
		auto time_info = std::dynamic_pointer_cast<core::backend::time>(ci);
		if (time_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(api::time_to_string(time_info->time_tp) + time_info->text));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::unknown_exception:
	{
		auto unknown_exception_info = std::dynamic_pointer_cast<core::backend::unknown_exception>(ci);
		if (unknown_exception_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(unknown_exception_info->code.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(unknown_exception_info->location));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::file_copy:
	{
		auto file_copy_info = std::dynamic_pointer_cast<core::backend::file_copy>(ci);
		if (file_copy_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(file_copy_info->code.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(file_copy_info->text));
			core::logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(file_copy_info->time_tp)));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::file_delete:
	{
		auto file_delete_info = std::dynamic_pointer_cast<core::backend::file_delete>(ci);
		if (file_delete_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(file_delete_info->code.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(file_delete_info->text));
			core::logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(file_delete_info->time_tp)));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	case core::backend::commands::directory_delete:
	{
		auto directory_delete_info = std::dynamic_pointer_cast<core::backend::directory_delete>(ci);
		if (directory_delete_info != nullptr)
		{
			core::logger::glb_sl->log_message(api::to_wide_string(directory_delete_info->code.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(directory_delete_info->text));
			core::logger::glb_sl->log_message(api::to_wide_string(
				api::time_to_string(directory_delete_info->time_tp)));
		}
		else
		{
			core::logger::glb_sl->log_message(api::to_wide_string(core::pointer_is_null_pkg.message()));
			core::logger::glb_sl->log_message(api::to_wide_string(api::get_location()));
		}
		break;
	}

	default:
		break;
	}
}
