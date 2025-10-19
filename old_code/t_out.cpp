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
	m_current_line = get_terminal_line();
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

std::size_t core::t_out::get_max_rows()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return static_cast<std::size_t>(csbi.srWindow.Bottom - csbi.srWindow.Top + 1); // Visible window height
}

void core::t_out::scrollConsoleWindow(int lines, bool scrollUp)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);

	SMALL_RECT scrollRect;
	COORD destPos;
	CHAR_INFO fill;

	// Define the rectangle to scroll (current console buffer)
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X - 1;
	scrollRect.Bottom = csbi.dwSize.Y - 1;

	// Set destination position for scrolling
	destPos.X = 0;
	destPos.Y = scrollUp ? -lines : lines; // Negative for up, positive for down

	// Fill character for empty space after scroll
	fill.Char.AsciiChar = ' ';
	fill.Attributes = csbi.wAttributes;

	// Perform the scroll
	ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, destPos, &fill);

	// Move cursor back to a reasonable position
	COORD cursorPos = { 0, csbi.dwCursorPosition.Y + (scrollUp ? -lines : lines) };
	SetConsoleCursorPosition(hConsole, cursorPos);
}

void core::t_out::set_current_line(std::size_t current_line)
{
	m_current_line = current_line;
	set_terminal_position(0, m_current_line);
}
