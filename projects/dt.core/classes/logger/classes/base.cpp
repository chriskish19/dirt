/**********************************************************/
//
// File: base.cpp
//
// Purpose: base.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_BASE_INCLUDE_PATH

core::logger::base::base(std::size_t nol) {
	// allocate the logs vector and fill
	m_logs_v = new std::vector<log*>;
	m_logs_v->reserve(nol);
	for (std::size_t i = 0; i < nol; ++i) {
		m_logs_v->push_back(new log(i));
	}
}

core::logger::base::~base()
{
	for (auto log : *m_logs_v) {
		if (log != nullptr) {
			delete log;
		}
	}
	
	if (m_logs_v != nullptr) {
		delete m_logs_v;
		m_logs_v = nullptr;
	}
}

void core::logger::base::set_log(core::logger::log* log_p)
{
	// cycle
	if (m_v_index < (m_logs_v->size() - 1)) {
		// next log
		m_v_index++;
	}
	else {
		// reset
		m_v_index = 0;
	}
}

int core::logger::base::count_new_line(const string& message)
{
	int count = 0;
	for (auto c : message) {
		if (c == '\n') {
			count++;
		}
	}
	return count;
}
