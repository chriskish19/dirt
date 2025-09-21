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
#include CORE_CMDLINE_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH


namespace core {
	class sim : public queue_system {
	public:
		~sim() {
			m_s_runner.store(false);

			if (m_test_set != nullptr) {
				delete m_test_set;
				m_test_set = nullptr;
			}
		}

		sim(const std::filesystem::path& test_dir) 
			:m_test_files(test_dir) {
			
			try {
				m_b_dir_it = std::filesystem::recursive_directory_iterator(m_test_files);
			}
			catch (const std::filesystem::filesystem_error& e) {
				output_em(std_filesystem_exception_caught_pkg);
				output_fse(e);
			}
			catch (...) {
				output_em(unknown_exception_caught_pkg);
			}
			
		}

		codes work(std::size_t ms);
	protected:
		std::unordered_set<directory_info>* m_test_set = new std::unordered_set<directory_info>();
		
		file_entry make_entry() {
			file_entry entry;
			entry.action = file_action::copy;
			entry.args_v = { args::copy,args::recursive,args::watch };
			entry.completed_action = {};
			entry.dst_p = std::filesystem::path();
			entry.p_di_set = m_test_set;
			

			if (m_b_dir_it != m_end_dir_it) {
				entry.src_p = m_b_dir_it->path();
				entry.src_s = m_b_dir_it->status();
				m_b_dir_it++;
			}
			
			return entry;
		}

		std::atomic<bool> m_s_runner = true;

		std::filesystem::path m_test_files;

		std::size_t m_entry_counter = 0;

		std::filesystem::recursive_directory_iterator m_b_dir_it;
		std::filesystem::recursive_directory_iterator m_end_dir_it = {};
	};
}