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

/* 
	testing values
	saves my ssd when testing
*/ 
#define DISABLE_COPY 1							// no copying when set to 1
#define DISABLE_DELETE 1						// no deleting when set to 1
#define TEST_ENTRY 1							// change application entry point
#define TEST_TIME_WAIT 30						// wait x seconds before flooding with work
#define TEST_FOLDER "C:\\linux\\Block"			// absolute system path on my c drive


/*
	file monitoring values
*/
#define BUFFER_SIZE 1024 * 1024 * 4				// 4 MB Buffer
#define BUFFER_TIME 10							// 10 seconds
#define MAX_THREADS 4							// 4 threads
#define MAX_QUEUE_SPLIT 1000					// 1000 files per queue


/*
	Logger values
*/
#define LOGGER_VERBOSE 0
#define LOGS 1000								// number of logs to cycle
#define LOG_LENGTH 512							// length of each log



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
	using w32_str_p = LPWSTR;			// wchar_t*
	using w32_str_cp = LPCWSTR;			// const wchar_t* (read only)
	namespace fs = std::filesystem;		// for less typing
	using ofstream = std::wofstream;
}



#else // ANSI

namespace core {
	using string = std::string;
	using ostringstream = std::ostringstream;
	using character = char;
	using w32_str_p = LPSTR;			// char*
	using w32_str_cp = LPCSTR;			// const char* (read only)
	namespace fs = std::filesystem;		// for less typing
	using ofstream = std::ofstream;
}


#define COUT std::cout
#define ROS(x) x
#define NARROW 1
#endif