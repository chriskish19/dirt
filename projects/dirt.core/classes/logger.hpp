/**********************************************************/
//
// File: logger.hpp
//
// Purpose: queue of logs to be displayed in terminal ui
//
// Project: dirt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH


#define LOGS 1000
#define LOG_LENGTH 512

namespace core {
	struct log {
		log(std::size_t log_index) :m_index(log_index) {
			message->reserve(LOG_LENGTH);

		}

		std::string* message = new std::string;

		~log() {
			if (message != nullptr) {
				delete message;
				message = nullptr;
			}
		}

		const std::size_t m_index;
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
		std::string time_stamped(const std::string& message);

		// prevent concurrent access to logs vec
		std::mutex m_v_mtx;
	};

	// use this class instead of functions
	class system_log : public base {
	public:
		system_log();
		~system_log();

		void log_message(const std::string& message);

		std::vector<std::string> latest();
	};

	// global system logger object, info logs
	extern std::unique_ptr<system_log> glb_sl;

	// global system logger object, error logs
	extern std::unique_ptr<system_log> glb_el;
}