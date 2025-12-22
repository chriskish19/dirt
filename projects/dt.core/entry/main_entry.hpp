/**********************************************************/
//
// File: main_entry.hpp
//
// Purpose: main classes for running dt
//
// Project: dt.core
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
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_TOUT_INCLUDE_PATH



namespace core {
	/************************************************/
	/*			Win32 GUI build objects				*/
	/************************************************/

	void gui_process_commands(std::shared_ptr<commands_info> ci);

	class backend : public process{
	public:
		backend(const std::vector<arg_entry>& v);
		core::codes run();
	protected:
	};


	class frontend : public dt_window{
	public:
	protected:
	};
	

	class gui_entry{
	public:
		gui_entry() {
			std::vector<core::arg_entry> empty;
			m_be = std::make_unique<backend>(empty);
			m_fe = std::make_unique<frontend>();
		}

		gui_entry(const std::vector<core::arg_entry>& v) {
			m_be = std::make_unique<backend>(v);
			m_fe = std::make_unique<frontend>();
		}

		void go() {
			std::jthread backend_run_t([this] { m_be->run(); });
			std::jthread backend_message_t([this](std::stop_token token) {
				backend_messages(token);
			});

			api::logger->fill();
			api::logger->display();

			// win32 message handler
			m_fe->message_pump();

			backend_message_t.request_stop();
		}


	protected:
		void backend_messages(std::stop_token token) {
			while (token.stop_requested() == false) {
				// timer here, seconds to wait time
				std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));
				auto q = m_be->get_current_queue();
				while (q.empty() == false && token.stop_requested() == false) {
					auto& command = q.front();
					gui_process_commands(command);
					q.pop();
				}
				std::cout << "\r" << BLUE << frames[i % 8] << " waiting..." << DEFAULT_COLOR << CLEAR_LINE;
				i++;
				
			}
		}
	
		std::unique_ptr<backend> m_be = nullptr;
		std::unique_ptr<frontend> m_fe = nullptr;

		const char frames[8] = { '⣾','⣽','⣻','⢿','⡿','⣟','⣯','⣷'};
		int i = 0;
	};



	/************************************************/
	/*			Terminal build objects				*/	
	/************************************************/
	
	/*
		for processing log messages and information
	*/
	void terminal_process_commands(std::shared_ptr<commands_info> ci);

	/*
		main object for running the terminal build
	*/
	class terminal_entry {
	public:
		terminal_entry(int argc, char* argv[])
			:m_argc(argc), m_argv(argv) {
		}

		codes init();
		void go();
	protected:
		std::unique_ptr<backend> m_be = nullptr;
		int m_argc = 0;
		char** m_argv = nullptr;
		std::vector<arg_entry> m_v;
	};


	/*
		tests the terminal build
	*/
	class test_terminal_entry : public entry_sim {
	public:
		test_terminal_entry()
			: entry_sim(TEST_FOLDER) {
		}
		void go();
	protected:
	};
	
}