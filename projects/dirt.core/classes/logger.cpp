#include "logger.hpp"
/**********************************************************/
//
// File: logger.cpp
//
// Purpose: logger.hpp definitions
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_LOGGER_INCLUDE_PATH


namespace core {
	// automatic system logger, info logs
	std::unique_ptr<system_log> glb_sl = nullptr;

	// automatic system logger, error logs
	std::unique_ptr<system_log> glb_el = nullptr;
}

core::base::base(std::size_t nol) {
	// allocate the logs vector and fill
	m_logs_v = new std::vector<log*>;
	m_logs_v->reserve(nol);
	for (std::size_t i = 0; i < nol; ++i) {
		m_logs_v->push_back(new log(i));
	}
}

core::base::~base()
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

void core::base::set_log(core::log* log_p)
{
	*log_p->message = time_stamped(*log_p->message);

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

std::string core::base::time_stamped(const std::string& message)
{
	try {
		auto now = std::chrono::system_clock::now();
		std::string time = std::format("[{}]", now);
		return time + message;
	}
	catch (const std::exception& e) {
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception caught..." << std::endl;
	}
	// exception thrown we return nothing
	return {};
}

core::system_log::system_log()
	:base(LOGS)
{

}

core::system_log::~system_log()
{

}

void core::system_log::log_message(const std::string& message)
{
	std::lock_guard<std::mutex> local_lock(m_v_mtx);
	
	// get current log
	log* log_p = nullptr;
	std::size_t index = 0;

	index = base::get_v_index();
	log_p = base::get_buffer()->at(index);
	*log_p->message = message;

	base::set_log(log_p);
}

void core::system_log::latest(std::vector<std::string>& log_v)
{
	std::lock_guard<std::mutex> local_lock(m_v_mtx);

	auto buffer = get_buffer();

	for (;m_latest_index < this->get_v_index(); ++m_latest_index) {
		log_v.push_back(*buffer->at(m_latest_index)->message);
	}

	// means the vector has looped once
	if (m_latest_index > this->get_v_index()) {
		m_latest_index = 0;
	}
}
