/**********************************************************/
//
// File: output.hpp
//
// Purpose: handle outputting to the gui or terminal
//
// Project: dt.core
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
		file_copy,
		file_delete,
		directory_delete,
	};

	struct commands_info {
		commands_info() = default;
		virtual std::shared_ptr<commands_info> clone() const = 0;
		virtual ~commands_info() = default;
		virtual commands command() const = 0;
	};

	struct progress_bar : public commands_info {
		float progress;

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<progress_bar>(*this);
		}

		commands command() const override {
			return commands::update_progress_bar;
		}
	};

	struct message : public commands_info {
		message(const std::string& message)
		:text(message){}
		
		std::string text;

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<message>(*this);
		}

		commands command() const override {
			return commands::message;
		}
	};

	struct time : public commands_info {
		time(const std::string& _message) 
		:text(api::time_now(_message)){}


		std::string text;
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();



		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<time>(*this);
		}

		commands command() const override {
			return commands::time_message;
		}
	};

	struct error : public commands_info {
		error(const code_pkg& c, const std::string& lo)
			:code(c),location(lo) {}
		
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();
		std::string location;
		code_pkg code;

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<error>(*this);
		}

		commands command() const override {
			return commands::error;
		}
	};

	struct fs_exception : public commands_info {
		fs_exception(const std::filesystem::filesystem_error& e)
		:text(api::text_output_fse(e)){}

		std::string text;

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<fs_exception>(*this);
		}

		commands command() const override {
			return commands::file_system_exception;
		}
	};

	struct unknown_exception : public commands_info {
		unknown_exception(const code_pkg& c,const std::string& lo) 
		:code(c),location(lo){}

		std::string location;
		code_pkg code;

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<unknown_exception>(*this);
		}

		commands command() const override {
			return commands::unknown_exception;
		}
	};

	struct file_copy : public commands_info {
		file_copy(const file_entry& entry,const code_pkg& c)
		:text(api::output_file_entry(entry)),code(c){}

		code_pkg code;
		std::string text;
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<file_copy>(*this);
		}

		commands command() const override {
			return commands::file_copy;
		}
	};

	struct file_delete : public commands_info {
		file_delete(const file_entry& entry, const code_pkg& c)
			:text(api::output_file_entry(entry)), code(c) {}

		code_pkg code;
		std::string text;
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<file_delete>(*this);
		}

		commands command() const override {
			return commands::file_delete;
		}
	};

	struct directory_delete : public commands_info {
		directory_delete(const file_entry& entry, const code_pkg& c)
			:text(api::output_file_entry(entry)), code(c) {}

		code_pkg code;
		std::string text;
		std::chrono::system_clock::time_point time_tp = std::chrono::system_clock::now();

		std::shared_ptr<commands_info> clone() const override {
			return std::make_shared<directory_delete>(*this);
		}

		commands command() const override {
			return commands::directory_delete;
		}
	};



	class backend_message_queue {
	public:
		backend_message_queue() = default;
		void add(const commands_info& ci);

		std::queue<std::shared_ptr<commands_info>> get_current_queue();
		std::size_t get_current_size() { return m_bq.size(); }
	protected:
		std::queue<std::shared_ptr<core::commands_info>> m_bq;
		std::mutex m_bq_mtx;
	};
}