
/**********************************************************/
//
// File: output.cpp
//
// Purpose: definitions
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_TOUT_INCLUDE_PATH


void core::backend::backend_message_queue::add(const core::backend::commands_info& ci)
{
	std::lock_guard<std::mutex> local_lock(m_bq_mtx);
	m_bq.push(ci.clone());
}

std::queue < std::shared_ptr<core::backend::commands_info> > core::backend::backend_message_queue::get_current_queue()
{
	std::lock_guard<std::mutex> local_lock(m_bq_mtx);
	std::queue <std::shared_ptr<core::backend::commands_info> > empty_q;
	empty_q.swap(m_bq);
	return empty_q;
}
