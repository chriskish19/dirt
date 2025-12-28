/**********************************************************/
//
// File: logger.hpp
//
// Purpose: queue of logs to be displayed in ui
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_STL_INCLUDE_PATH

namespace core {
	namespace backend {
		struct log {
			log(std::size_t log_index);
			~log();
			const std::size_t m_index;
			std::string* m_message = new std::string;
		};

		class base {
		public:
			base(std::size_t nol);											// nol : number of logs
			~base();
			void set_log(log* log_p);										// log a message
			std::size_t get_v_index() { return m_v_index; }					// get next index position in logs vec for an unset log
			std::vector<log*>* get_buffer() { return m_logs_v; }			// get logs_V pointer
			std::mutex m_v_mtx;												// prevent concurrent access to logs vec and m_message_queue
		protected:
			std::vector<log*>* m_logs_v = nullptr;							// vector used for each log
			std::size_t m_v_index = 0;										// this index is always set to the next log, not the currently last set log, its a vector index
			std::string time_stamped(const std::string& message);			// returns the message with a time on it ([2025-05-09-14:00:30...])
		};

		class system_log : public base {
		public:
			system_log();
			~system_log();
			void fill();
			void log_message(const std::string& message);
			void display();
		protected:
			base display_logger{ LOGS };
		};
	}
}