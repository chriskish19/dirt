/***************************************
*  File: base.hpp (base logger)
*
*  Purpose: handles underlying log system
*
*  Project: dt.core
* *************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_DEFINES_INCLUDE_PATH


namespace core {
	namespace logger {


		struct log {
			log(std::size_t log_index) :m_index(log_index) {
				message->reserve(LOG_LENGTH);

			}

			string* message = new string;
			RECT* window_position = new RECT(0, 0, 0, 0);

			~log() {
				if (message != nullptr) {
					delete message;
					message = nullptr;
				}

				if (window_position != nullptr) {
					delete window_position;
					window_position = nullptr;
				}
			}

			std::atomic<bool> m_fresh_message = true;
			const std::size_t m_index;
			std::size_t m_lines = 1;
			std::size_t m_height = LOGGER_FONT_SIZE;
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
		protected:
			// vector used for each log
			std::vector<log*>* m_logs_v = nullptr;

			// this index is always set to the next log
			// not the currently last set log
			// its a vector index
			std::size_t m_v_index = 0;

			// simple time stamp a message
			// returns the message with a time on it ([2025-05-09-14:00:30...])
			string time_stamped(const string& message);

			// prevent concurrent access to logs vec
			std::mutex m_v_mtx;
		};
	}
}