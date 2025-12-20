/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_SIM_INCLUDE_PATH
#include CORE_SIM_API_INCLUDE_PATH
#include CORE_WINDOW_INCLUDE_PATH
#include CORE_UI_INCLUDE_PATH



namespace core {
	class backend : public process{
	public:
		backend(const std::vector<arg_entry>& v);

		// launch the backend
		core::codes run();
	protected:
	
	
	};




	class frontend {
	public:




	protected:
		window main_window;
		ui main_window_ui;
	};
	
	
	class terminal_entry {
	public:
		terminal_entry(int argc, char* argv[]) 
		:m_argc(argc),m_argv(argv){}

		codes init() {
			{
				codes code;
				m_v = cmd_line(m_argc, m_argv, &code);
				if (code != codes::success) {
					auto error_code = match_code(code);
					std::cout << error_code.m_s_code << '\n';
					return code;
				}
			}

			{
				codes code = validate(m_v);
				if (code != codes::success) {
					auto error_code = match_code(code);
					std::cout << error_code.m_s_code << '\n';
					return code;
				}
			}

			m_be = std::make_unique<backend>(m_v);
		}

		void go() {
			std::jthread backend_run_t(&backend::run, m_be);

			while (true) {
				// timer here, seconds to wait time
				std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));

				auto q = m_be->get_current_queue();

				while (q.empty() == false) {
					auto& command = q.front();

					process_commands(command);
				}
			}
		}

		void process_commands(const commands_info& ci) {
			switch (ci.command) {
				case commands::error:
				{
					auto& error_info = dynamic_cast<const error&>(ci);
					std::cout << error_info.code.m_s_code << '\n';
					std::cout << error_info.location << '\n';
					std::cout << error_info.time_tp << '\n';
					break;
				}

				case commands::file_system_exception:
				{
					auto& exception_info = dynamic_cast<const fs_exception&>(ci);
					std::cout << exception_info.text << '\n';
					break;
				}

				case commands::message:
				{
					auto& message_info = dynamic_cast<const message&>(ci);
					std::cout << message_info.text << '\n';
					break;
				}

				case commands::time_message:
				{
					auto& time_info = dynamic_cast<const time&>(ci);
					std::cout << time_info.text << '\n';
					break;
				}

				case commands::unknown_exception:
				{
					auto& unknown_exception_info = dynamic_cast<const unknown_exception&>(ci);
					std::cout << unknown_exception_info.code.m_s_code << '\n';
					std::cout << unknown_exception_info.location << '\n';
				}

				default:
					break;
			}
		}
	protected:
		std::unique_ptr<backend> m_be = nullptr;

		int m_argc = 0;
		char** m_argv = nullptr;

		std::vector<arg_entry> m_v;

	};


	class test_terminal_entry : public entry_sim {
	public:
		test_terminal_entry() = default;

		void go() {
			std::jthread worker_t(&work, FLOOD);

			while (true) {
				// timer here, seconds to wait time
				std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));

				auto q = get_current_queue();

				while (q.empty() == false) {
					auto& command = q.front();

					process_commands(command);
				}
			}
		}

		void process_commands(const commands_info& ci) {
			switch (ci.command) {
			case commands::error:
			{
				auto& error_info = dynamic_cast<const error&>(ci);
				std::cout << error_info.code.m_s_code << '\n';
				std::cout << error_info.location << '\n';
				std::cout << error_info.time_tp << '\n';
				break;
			}

			case commands::file_system_exception:
			{
				auto& exception_info = dynamic_cast<const fs_exception&>(ci);
				std::cout << exception_info.text << '\n';
				break;
			}

			case commands::message:
			{
				auto& message_info = dynamic_cast<const message&>(ci);
				std::cout << message_info.text << '\n';
				break;
			}

			case commands::time_message:
			{
				auto& time_info = dynamic_cast<const time&>(ci);
				std::cout << time_info.text << '\n';
				break;
			}

			case commands::unknown_exception:
			{
				auto& unknown_exception_info = dynamic_cast<const unknown_exception&>(ci);
				std::cout << unknown_exception_info.code.m_s_code << '\n';
				std::cout << unknown_exception_info.location << '\n';
			}

			default:
				break;
			}
		}
	protected:

	};






	class gui_entry {
	public:
		gui_entry() {
			

		}

		void go();

	protected:
		std::unique_ptr<backend> m_be = nullptr;
		std::unique_ptr<frontend> m_fe = nullptr;
	};

	
}