/**********************************************************/
//
// File: defines.hpp
//
// Purpose: define macros used throughout dirt.core
//
// Project: dirt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH


/*
	building dirt
*/
#define TERMINAL_BUILD 0						// build dirt for running as a terminal app
#define WIN32_GUI_BUILD 1						// build dirt with a gui
#define UNDER_CONSTRUCTION 1					// files being currently worked on
#define WIN32_GUI_AND_TERMINAL 0				// both win32 gui and terminal appear

/* 
	assest paths
*/
#define DT_ICON_PATH "C:\\Users\\chris\\source\\repos\\dirt\\assets\\dt_icon.ico"


/*
	terminal colors
*/
constexpr const char* BRIGHT_GREEN = "\033[92m"; 
constexpr const char* DEFAULT_COLOR = "\033[0m";
constexpr const char* BLUE = "\033[34m";
constexpr const char* CLEAR_LINE = "\r\033[K";

/* 
	testing values
	saves my ssd when testing
*/ 
#define DISABLE_COPY 1							// no copying when set to 1
#define DISABLE_DELETE 1						// no deleting when set to 1
#define TEST_ENTRY 1							// change application entry point
#define TEST_TIME_WAIT 30						// wait x seconds before flooding with work
#define TEST_FOLDER "C:\\linux"					// absolute system path on my c drive
#define FLOOD 10								// generate entries for x ms

/*
	file monitoring values
*/
#define BUFFER_SIZE 1024 * 1024 * 4				// 4 MB Buffer
#define BUFFER_TIME 10							// 10 seconds
#define MAX_THREADS 4							// 4 threads
#define MAX_QUEUE_SPLIT 1000					// 1000 files per queue
#define GUI_SYNC_INTERVAL 1000					// in ms

/*
	Logger values
*/
#define ENABLE_API_LOGS 0						// get logs from api
#define LOGS 1000								// number of logs to cycle
#define LOG_LENGTH 512							// length of each log
#define LOGGER_LINES 1000
#define LOGGER_FONT_SIZE 14
#define ENABLE_VERBOSE_LOGGING 1

/*
	Strings
*/
#define MAX_STRING 512
#define LB_MAX_STRING 256


/*
	handle unicode strings/ ansi string types
*/
#if defined(UNICODE) || defined(_UNICODE)
#define COUT std::wcout

// read only string, prepends L"string literal"
#define ROS(x) L##x

// using wide strings
#define WIDE 1



// define types
namespace core{
	using string = std::wstring;
	using ostringstream = std::wostringstream;
	using character = wchar_t;
	using character_p = LPWSTR;					// wchar_t*
	using const_character_p = LPCWSTR;			// const wchar_t* (read only)
	namespace fs = std::filesystem;				// for less typing
	using ofstream = std::wofstream;
	using istringstream = std::wistringstream;
}

#else // ANSI

namespace core {
	using string = std::string;
	using ostringstream = std::ostringstream;
	using character = char;
	using character_p = LPSTR;					// char*
	using const_character_p = LPCSTR;			// const char* (read only)
	namespace fs = std::filesystem;				// for less typing
	using ofstream = std::ofstream;
}


#define COUT std::cout
#define ROS(x) x
#define NARROW 1
#endif