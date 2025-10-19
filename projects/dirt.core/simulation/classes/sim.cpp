
/**********************************************************/
//
// File: sim.cpp
//
// Purpose: classes that simulate work and test system
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_SIM_INCLUDE_PATH

core::entry_sim::~entry_sim()
{
	m_s_runner.store(false);

	if (m_test_set != nullptr) {
		delete m_test_set;
		m_test_set = nullptr;
	}
}

core::entry_sim::entry_sim(const std::filesystem::path& test_dir)
	:m_test_files(test_dir)
{
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

core::codes core::entry_sim::work(std::size_t ms) {
	std::thread queue_sys_t(&queue_system::process_entry, this);

	while (m_s_runner.load() == true) {
		auto start = std::chrono::steady_clock::now();
		auto duration = std::chrono::milliseconds(ms);
		std::chrono::milliseconds elapsed = {};
		while (elapsed <= duration) {
			// Get current time and elapsed time
			auto now = std::chrono::steady_clock::now();
			elapsed = duration_cast<std::chrono::milliseconds>(now - start);

			auto entry = make_entry();
			add_entry(entry);
		}

		// signal queue system
		m_launch_b.store(true);
		m_launch_cv.notify_all();

		std::this_thread::sleep_for(std::chrono::seconds(TEST_TIME_WAIT));
	}

	if (queue_sys_t.joinable()) {
		queue_sys_t.join();
	}

	return codes::success;
}

core::file_entry core::entry_sim::make_entry()
{
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
