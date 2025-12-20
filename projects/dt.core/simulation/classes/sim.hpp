/**********************************************************/
//
// File: sim.hpp
//
// Purpose: classes that simulate work and test system
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH


namespace core {
	class entry_sim : public queue_system {
	public:
		~entry_sim();
		entry_sim(const std::filesystem::path& test_dir);

		codes work(std::size_t ms);
	protected:
		std::unordered_set<directory_info>* m_test_set = new std::unordered_set<directory_info>();
		
		file_entry make_entry();

		std::atomic<bool> m_s_runner = true;

		std::filesystem::path m_test_files;

		std::size_t m_entry_counter = 0;

		std::filesystem::recursive_directory_iterator m_b_dir_it;
		std::filesystem::recursive_directory_iterator m_end_dir_it = {};
	};
}