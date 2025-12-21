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
	/*
		for the terminal build
	*/
	void terminal_process_commands(std::shared_ptr<commands_info> ci);


	class backend : public process{
	public:
		backend(const std::vector<arg_entry>& v);

		// launch the backend
		core::codes run();
	protected:
	
	};



#if! UNDER_CONSTRUCTION
	class frontend {
	public:
	protected:
		window main_window;
		ui main_window_ui;
	};
#endif	
	
	class terminal_entry {
	public:
		terminal_entry(int argc, char* argv[]) 
		:m_argc(argc),m_argv(argv){}

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
			: entry_sim(TEST_FOLDER) {}

		void go();
	protected:

	};



#if! UNDER_CONSTRUCTION
	class gui_entry {
	public:
		gui_entry() {}
		void go();
	protected:
		std::unique_ptr<backend> m_be = nullptr;
		std::unique_ptr<frontend> m_fe = nullptr;
	};
#endif
	
}