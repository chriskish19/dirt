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

// dt core headers
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


// Logging Window Library
#include LOGGER_NAMES_INCLUDE
#include LOGGER_ALL_LOGS_INCLUDE_PATH


namespace core {
	/************************************************/
	/*				Drawing objects					*/
	/************************************************/
	

	/*
		draws a loading bar in the terminal
	*/
	class loading_bar {
	public:
		void draw() {
			// Get pointer to the start of the current 3-byte sequence
			const char8_t* current_frame_start = frames + (frame_index * bytes_per_frame);

			std::cout << "\r" << BLUE;
			std::cout.write(reinterpret_cast<const char*>(current_frame_start), bytes_per_frame);
			std::cout << " waiting..." << DEFAULT_COLOR;

			// Advance to next frame
			frame_index = (frame_index + 1) % num_frames;
		}
	protected:
		int frame_index = 0;
		const int num_frames = 8;
		const char8_t frames[25] = u8"⣾⣽⣻⢿⡿⣟⣯⣷";
		const int bytes_per_frame = 3;
	};
	
	


	
	/************************************************/
	/*			Win32 GUI build objects				*/
	/************************************************/


	class backend : public process{
	public:
		backend(const std::vector<arg_entry>& v);
		core::codes run(std::stop_token token);
	protected:
	};


	class frontend : public dt_window{
	public:
	protected:
	};
	

	class gui_with_terminal_entry {
	public:
		gui_with_terminal_entry() {
			std::vector<core::arg_entry> empty;
			m_be = std::make_unique<backend>(empty);
			m_fe = std::make_unique<frontend>();
		}

		gui_with_terminal_entry(const std::vector<core::arg_entry>& v) {
			m_be = std::make_unique<backend>(v);
			m_fe = std::make_unique<frontend>();
		}

		void go() {
			std::jthread backend_run_t([this](std::stop_token token) { m_be->run(token); });
			std::jthread backend_message_t([this](std::stop_token token) {
				backend_messages(token);
				});

			api::logger->fill();
			api::logger->display();

			// win32 message handler
			m_fe->message_pump();

			//PostQueuedCompletionStatus(m_be->m_hCompletionPort, 0, 0, nullptr);
		}


	protected:
		void process_commands(std::shared_ptr<commands_info> ci);

		void backend_messages(std::stop_token token) {
			loading_bar progress;

			while (token.stop_requested() == false) {
				// timer here, seconds to wait time
				std::this_thread::sleep_for(std::chrono::milliseconds(GUI_SYNC_INTERVAL));
				auto q = m_be->get_current_queue();

				if (q.empty() == false) {
					auto& command = q.front();
					std::cout << '\n';
					process_commands(command);
					q.pop();
				}

				while (q.empty() == false && token.stop_requested() == false) {
					auto& command = q.front();
					process_commands(command);
					q.pop();
				}

				progress.draw();
			}
		}

		std::unique_ptr<backend> m_be = nullptr;
		std::unique_ptr<frontend> m_fe = nullptr;
	};


	class gui_entry{
	public:
		gui_entry() {
			std::vector<core::arg_entry> empty;
			m_be = std::make_unique<backend>(empty);
			m_fe = std::make_unique<frontend>();
			logger::glb_sl = std::make_unique<logger::system_log>();
		}

		gui_entry(const std::vector<core::arg_entry>& v) {
			m_be = std::make_unique<backend>(v);
			m_fe = std::make_unique<frontend>();
			logger::glb_sl = std::make_unique<logger::system_log>();
		}

		void go() {
			std::jthread backend_run_t([this](std::stop_token token) { m_be->run(token); });
			std::jthread backend_message_t([this](std::stop_token token) {
				backend_messages(token);
			});

			api::logger->fill();
			api::logger->display();

			// win32 message handler
			m_fe->message_pump();

			backend_message_t.request_stop();
			backend_run_t.request_stop();
			PostQueuedCompletionStatus(m_be->m_hCompletionPort, 0, 0, nullptr);
		}


	protected:
		void gui_process_commands(std::shared_ptr<commands_info> ci);

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
			}
		}
	
		std::unique_ptr<backend> m_be = nullptr;
		std::unique_ptr<frontend> m_fe = nullptr;
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