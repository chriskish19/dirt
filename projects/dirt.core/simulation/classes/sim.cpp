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

core::codes core::sim::work(std::size_t ms) {
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