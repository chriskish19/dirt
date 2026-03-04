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
#include CORE_MESSAGES_INCLUDE_PATH

core::main::messages::messages(build type)
{
	switch (type) {
#if !TERMINAL_BUILD
	case build::gui:
		m_frontend = std::make_unique<core::frontend::gui>();
		break;
	case build::gui_with_terminal:
		m_frontend = std::make_unique<core::frontend::gui_with_terminal>();
		break;
#else
	case build::terminal:
		m_frontend = std::make_unique<core::frontend::terminal>();
		break;
#endif
	default:
		break;
	}

	m_backend = std::make_unique<core::backend::Cbackend>(m_entry_v);
}

core::main::messages::messages(build type, int argc, char* argv[])
{
	switch (type) {
#if !TERMINAL_BUILD
	case build::gui:
		m_frontend = std::make_unique<core::frontend::gui>();
		break;
	case build::gui_with_terminal:
		m_frontend = std::make_unique<core::frontend::gui_with_terminal>();
		break;
#else
	case build::terminal:
		m_frontend = std::make_unique<core::frontend::terminal>();
		break;
#endif
	default:
		break;
	}

	{
		core::codes code;
		m_entry_v = api::cmd_line(argc, argv, &code);
		if (code != core::codes::success) {
#if !WIN32_GUI_BUILD
			std::cout << api::match_code(code).message() << '\n';
			std::cout << api::get_location() << '\n';
#else
			api::output_cp(api::match_code(code));
#endif
		}
	}

	{
		core::codes code = api::validate(m_entry_v);
		if (code != core::codes::success) {
#if !WIN32_GUI_BUILD
			std::cout << api::match_code(code).message() << '\n';
			std::cout << api::get_location() << '\n';
#else
			api::output_cp(api::match_code(code));
#endif
		}
	}


	m_backend = std::make_unique<core::backend::Cbackend>(m_entry_v);
}

void core::main::messages::process()
{
	while (m_run_messages.load() == true) {
		// timer here, seconds to wait time
		std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));
		
		auto q = m_backend->get_current_queue();
		while (q.empty() == false && m_run_messages.load() == true) {
			auto& command = q.front();
			m_frontend->process_commands(command);
			q.pop();
		}

		m_frontend->draw_progress();
	}
}
