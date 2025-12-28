/**********************************************************/
//
// File: logger.cpp
//
// Purpose: logger.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_LOGGER_INCLUDE_PATH

core::backend::base::base(std::size_t nol) {
	// allocate the logs vector and fill
	m_logs_v = new std::vector<log*>;
	m_logs_v->reserve(nol);
	
	for (std::size_t i = 0; i < nol; ++i) {
		m_logs_v->push_back(new log(i));
	}
}

core::backend::base::~base()
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

void core::backend::base::set_log(core::backend::log* log_p)
{
	*log_p->m_message = time_stamped(*log_p->m_message);

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

std::string core::backend::base::time_stamped(const std::string& message)
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

core::backend::system_log::system_log()
:base(LOGS){}

core::backend::system_log::~system_log(){}

void core::backend::system_log::log_message(const std::string& message)
{
	std::lock_guard<std::mutex> local_lock(m_v_mtx);
	
	// get current log
	log* log_p = nullptr;
	std::size_t index = base::get_v_index(); 

	log_p = base::get_buffer()->at(index);
	*log_p->m_message = message;

	base::set_log(log_p);
}

void core::backend::system_log::display()
{
	std::lock_guard<std::mutex> local_lock(display_logger.m_v_mtx);
	for (auto log : *display_logger.get_buffer()) {
		if(log->m_message->empty() == false)
			std::cout << *log->m_message << '\n';
		else {
			break;
		}
	}
}

void core::backend::system_log::fill()
{
	std::lock_guard<std::mutex> local_lock(m_v_mtx);
	display_logger.get_buffer()->swap(*get_buffer());
}

core::backend::log::log(std::size_t log_index)
	:m_index(log_index)
{
	m_message->reserve(LOG_LENGTH);
}

core::backend::log::~log()
{
	if (m_message != nullptr) {
		delete m_message;
		m_message = nullptr;
	}
}