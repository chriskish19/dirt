
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
			// nol : number of logs
			base(std::size_t nol);
			~base();

			// log a message
			void set_log(log* log_p);

			// get next index position in logs vec for an unset log
			std::size_t get_v_index() { return m_v_index; }

			// get logs_V pointer
			std::vector<log*>* get_buffer() { return m_logs_v; }

			// prevent concurrent access to logs vec and m_message_queue
			std::mutex m_v_mtx;
		protected:
			// vector used for each log
			std::vector<log*>* m_logs_v = nullptr;

			// this index is always set to the next log
			// not the currently last set log
			// its a vector index
			std::size_t m_v_index = 0;

			// simple time stamp a message
			// returns the message with a time on it ([2025-05-09-14:00:30...])
			std::string time_stamped(const std::string& message);
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