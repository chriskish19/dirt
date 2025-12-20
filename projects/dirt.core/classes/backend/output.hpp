/**********************************************************/
//
// File: output.hpp
//
// Purpose: handle outputting to the gui or terminal
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH	
#include CORE_API_INCLUDE_PATH


namespace core {
	codes main_output(const std::string& log);

	enum class commands {
		message,
		update_progress_bar,
		time_message,
		error,
		file_system_exception,
		unknown_exception,
	};

	struct commands_info {
		commands command;

		virtual ~commands_info() = default;
	};

	struct progress_bar : public commands_info {
		float progress;
	};

	struct message : public commands_info {
		std::string text;
	};

	struct time : public message {
		time(const std::string& message) {
			text = time_now(message);
			command = commands::time_message;
		}

		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();
	};

	struct error : public commands_info {
		error(const code_pkg& c, const std::string& lo)
			:code(c),location(lo) {
			command = commands::error;
		}
		
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();
		std::string location;
		code_pkg code;
	};

	struct fs_exception : public commands_info {
		fs_exception(const std::filesystem::filesystem_error& e)
		:text(text_output_fse(e)){
			command = commands::file_system_exception;
		}

		std::string text;
	};

	struct unknown_exception : public commands_info {
		unknown_exception(const code_pkg& c,const std::string& lo) 
		:code(c),location(lo){
			command = commands::unknown_exception;
		}

		std::string location;
		code_pkg code;
	};

	class backend_message_queue {
	public:
		backend_message_queue() = default;
		void add(const commands_info& ci);

		std::queue<commands_info> get_current_queue();
		std::size_t get_current_size() { return m_bq.size(); }
	protected:
		std::queue<commands_info> m_bq;
		std::mutex m_bq_mtx;
	};
}