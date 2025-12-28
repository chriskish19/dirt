/**********************************************************/
//
// File: m_q_sys.cpp (message queue system)
// 
// Purpose: m_q_sys.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_MQSYS_INCLUDE_PATH

core::logger::log_q::log_q()
{
	m_log_q = new std::queue<log*>;
	m_log_q_buffer = new std::queue<log*>;
}

core::logger::log_q::~log_q()
{
	while (m_log_q->empty() == false) {
		auto log = m_log_q->front();
		if (log != nullptr) {
			delete log;
		}
		m_log_q->pop();
	}
	
	while (m_log_q_buffer->empty() == false) {
		auto log = m_log_q_buffer->front();
		if (log != nullptr) {
			delete log;
		}
		m_log_q_buffer->pop();
	}
	

	if (m_log_q != nullptr) {
		delete m_log_q;
		m_log_q = nullptr;
	}

	if (m_log_q_buffer != nullptr) {
		delete m_log_q_buffer;
		m_log_q_buffer = nullptr;
	}
}

void core::logger::log_q::add_message(core::logger::log* log)
{
	std::unique_lock<std::mutex> local_lock(m_q_mtx);
	m_log_q_buffer->push(log);
}

void core::logger::log_q::exit()
{
	m_run_pm.store(false);
	m_signal_b.store(true);
	m_signal_cv.notify_all();
}


