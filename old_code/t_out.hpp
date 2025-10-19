/**********************************************************/
//
// File: t_out.hpp (terminal out)
//
// Purpose: handle drawing and output to the terminal
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH

#include <Windows.h>

namespace core {
	class t_out {
	public:
		t_out();

		template<typename t>
		void progress_bar(t current, t total = 100, t width = 50,t line=0,t thread_number=0);
		static void enable_vt_mode();
		void log_out(const std::string& log);
		void clear_terminal();
		std::size_t get_terminal_line();
		void set_terminal_position(std::size_t x,std::size_t y);
		std::size_t get_current_line() { return m_current_line; }
		std::size_t get_max_rows();
		void scrollConsoleWindow(int lines, bool scrollUp);
		void set_current_line(std::size_t current_line);
	protected:
		std::atomic<std::size_t> m_current_line = 0;
		std::mutex m_log_mtx;
		std::mutex m_line_mtx;
	};


	template<typename t>
	inline void t_out::progress_bar(t current, t total, t width,t line,t thread_number)
	{
		set_current_line(line);
		
		float percentage = static_cast<float>(current) / total * 100;
		int filled = static_cast<int>(width * current / total);

		// Format the progress bar with thread ID
		std::cout << "Thread#" << thread_number << " Progress: [";
		for (int i = 0; i < width; ++i) {
			std::cout << "\033[32m"; // Green
			std::cout << (i < filled ? "=" : "-");
			std::cout << "\033[0m"; // Reset color
		}
		std::cout << "] " << std::fixed << std::setprecision(1) << percentage << "% " << current << "/" << total;
		std::cout << std::endl;
	}

}