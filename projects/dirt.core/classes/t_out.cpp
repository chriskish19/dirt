#include "t_out.hpp"
/**********************************************************/
//
// File: t_out.cpp
//
// Purpose: t_out.hpp definitions
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_TOUT_INCLUDE_PATH

core::t_out::t_out()
{

}

void core::t_out::enable_vt_mode() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void core::t_out::log_out(const std::string& log)
{
	std::lock_guard<std::mutex> local_lock(m_log_mtx);
	std::cout << log;
}

void core::t_out::clear_terminal()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coordScreen, &cCharsWritten);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

std::size_t core::t_out::get_terminal_line()
{
	std::lock_guard<std::mutex> local_lock(m_line_mtx);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return static_cast<std::size_t>(csbi.dwCursorPosition.Y+1);
}

void core::t_out::set_terminal_position(std::size_t x, std::size_t y)
{
	// Move cursor to top of progress bars
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)x, (SHORT)y};
	SetConsoleCursorPosition(hConsole, pos);
}
