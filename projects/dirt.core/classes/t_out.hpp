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
		void progress_bar(t current, t total = 100, t width = 50,t line=0);

		static void enable_vt_mode();
	protected:
		
		void clear_terminal();
	};


	template<typename t>
	inline void t_out::progress_bar(t current, t total, t width,t line)
	{
		// Move cursor to top of progress bars
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos = { 0, (SHORT)line};
		SetConsoleCursorPosition(hConsole, pos);
		
		float percentage = static_cast<float>(current) / total * 100;
		int filled = static_cast<int>(width * current / total);

		// Format the progress bar with thread ID
		std::cout << "Thread#" << line << " Progress: [";
		for (int i = 0; i < width; ++i) {
			std::cout << "\033[32m"; // Green
			std::cout << (i < filled ? "=" : "-");
			std::cout << "\033[0m"; // Reset color
		}
		std::cout << "] " << std::fixed << std::setprecision(1) << percentage << "%";
		std::cout << "\n";
	}
}