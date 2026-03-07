/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_FRONTEND_INCLUDE_PATH

#if !TERMINAL_BUILD

void core::frontend::gui_with_terminal::draw_progress()
{
	progress.draw();
}

void core::frontend::gui_with_terminal::process_commands(std::shared_ptr<core::backend::commands_info> ci)
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
			std::cout << message_info->text;
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
			std::cout << "[" << time_info->time_tp << "]" << time_info->text;
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

	case core::backend::commands::new_backend:
	{

		break;
	}

	default:
		break;
	}
}

core::frontend::gui::~gui()
{
	if (progress != nullptr) {
		delete progress;
	}
}

void core::frontend::gui::draw_progress()
{
	progress->draw();
}

void core::frontend::gui::process_commands(std::shared_ptr<core::backend::commands_info> ci)
{
	switch (ci->command())
	{
	case core::backend::commands::update_progress_bar:
	{
		auto progress_bar_info = std::dynamic_pointer_cast<core::backend::progress_bar>(ci);
		if (progress_bar_info != nullptr) {
			switch (progress_bar_info->bar_number) {
			case core::backend::progress_bar::id::one:
				SendMessage(get_ui_p()->m_progress_bar1.get_bar_handle(), PBM_SETPOS, progress_bar_info->progress, 0);
				break;
			case core::backend::progress_bar::id::two:
				SendMessage(get_ui_p()->m_progress_bar2.get_bar_handle(), PBM_SETPOS, progress_bar_info->progress, 0);
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

#else

void core::frontend::terminal::draw_progress()
{
	progress.draw();
}

void core::frontend::terminal::process_commands(std::shared_ptr<core::backend::commands_info> ci)
{
	switch (ci->command()) {
	case core::backend::commands::terminal_message:
	{
		auto t_message_info = std::dynamic_pointer_cast<core::backend::terminal_message>(ci);
		if (t_message_info != nullptr) {
			progress.set_message(t_message_info->text);
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		break;
	}

	case core::backend::commands::update_progress_bar:
	{
		auto progress_bar_info = std::dynamic_pointer_cast<core::backend::progress_bar>(ci);
		if (progress_bar_info != nullptr) {
			progress.set_message(std::format(" Processing {:.1f}%", progress_bar_info->progress));
			progress.draw();
		}
		else {
			std::cout << core::pointer_is_null_pkg.message() << '\n';
			std::cout << api::get_location() << '\n';
		}
		break;
	}

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
			std::cout << message_info->text;
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
			std::cout << "[" << time_info->time_tp << "]" << time_info->text;
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

#endif