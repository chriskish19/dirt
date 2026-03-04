/**********************************************************/
//
// File: sim.hpp
//
// Purpose: classes that simulate work and test system
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_FRONTEND_INCLUDE_PATH
#include CORE_BACKEND_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH


namespace test {
	class base : public core::backend::Cbackend {
	public:
		virtual ~base();
		base(const std::filesystem::path& test_dir);
		core::codes work(std::size_t ms);
		virtual void go() = 0;
		virtual void exit() = 0;
	protected:
		std::unordered_set<core::directory_info>* m_test_set = new std::unordered_set<core::directory_info>();
		core::file_entry make_entry();
		std::atomic<bool> m_s_runner = true;
		std::filesystem::path m_test_files;
		std::size_t m_entry_counter = 0;
		std::filesystem::recursive_directory_iterator m_b_dir_it;
		std::filesystem::recursive_directory_iterator m_end_dir_it = {};
	};


#if TERMINAL_BUILD
	class main : public core::frontend::terminal, public base {
	public:
		main():base(TEST_FOLDER){}

		void go() override {

		}

	protected:
		void exit() override {

		}
	};
#endif

#if WIN32_GUI_BUILD
	class main : public core::backend::Cbackend, public core::frontend::gui {
	public:



	protected:

	};



#endif

#if WIN32_GUI_AND_TERMINAL
	class main : public core::backend::Cbackend, public core::frontend::gui_with_terminal {
	public:



	protected:

	};

#endif
}
