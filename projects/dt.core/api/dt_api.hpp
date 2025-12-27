/**********************************************************/
//
// File: dt_api.hpp
//
// Purpose: helper functions
//
// Project: dt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_WIN32_INCLUDE_PATH



namespace api {


#if ENABLE_API_LOGS
	extern std::unique_ptr<core::system_log> logger;
#endif


	/*
		convert wWinMain command line argvs from wide to narrow strings
	*/
	std::vector<std::string> convert_cmdline_args_to_utf8(LPWSTR* wide_argv, int argc);


	/* 
		returns the package (class arg_pkg which is an arg enum and the arg enum as a string)
		given an argument from args enum. See header file CORE_ARGS_INCLUDE_PATH.
	*/
	core::arg_pkg match_arg_enum(core::args arg);
	
	
	/*
		returns an arg package from a given string
		simple if statements comparing strings kind of expensive
	*/
	core::arg_pkg match_s_arg(const std::string& arg);
	
	
	/*
		returns a code package that matches the given code.
		each error code has a package that includes an explanation string that
		can be used to output a message to the terminal
	*/
	core::code_pkg match_code(core::codes code);
	
	
	/*
		returns a formatted string with location information
		file name, column number, function name, line number
		Works from the function call site.
	*/
	std::string get_location(std::source_location sl = std::source_location::current());
	
	
	/*
		outputs an error message with location data, if logger is enabled
	*/
	void output_em(const core::code_pkg cp, const std::string location = api::get_location());


	/*
		outputs a file system exception message, if logger is enabled
	*/
	void output_fse(const std::filesystem::filesystem_error& e);


	/*
		checks that the given args have no conflicts and matches the groups
		see README for group argument combos
	*/
	std::vector<core::args> validate_args(const std::vector<core::arg_pkg>& args);
	
	
	/*
		converts the code in a file to a vector of arg_entries see README for example. 
	*/
	std::vector<core::arg_entry> parse_file(const std::filesystem::path& p, core::codes* code_p);
	
	
	/*
		returns a vector with only arguments (args) not the full class package
	*/
	std::vector<core::args> arg_pkg_to_args(const std::vector<core::arg_pkg>& args_pkg_v);
	
	
	/*
		uses the group classes to check for valid entries
		returns false if e is invalid
	*/
	bool validate_entry(const core::arg_entry& e);
	
	
	/*
		a returned string of win32 last error, detailed message
	*/
	std::string get_last_error_w32();
	
	
	/*
		returns string information for the arg entry e
	*/
	std::string output_entry(const core::arg_entry& e);
	
	
	/*
		outputs a file system exception error information to a returned string
	*/
	std::string text_output_fse(const std::filesystem::filesystem_error& e);
	
	
	/*
		returns a count of regular file's in a given directory 
	*/
	std::uintmax_t file_numbers(const std::filesystem::path& p);
	
	
	/*
		from a given path, a set is returned with all the directory entries including number of regular files
	*/
	std::unordered_set<core::directory_info> get_all_directories(const std::filesystem::path& p);
	
	
	/*
		copies files in a directory src to dst and updates existing files, but not sub directories
	*/
	core::codes copy_directory_only(const std::filesystem::path& dst, const std::filesystem::path& src);


	/*
		copies files in a directory including sub directories from src to dst
	*/
	core::codes copy_directory_recursive(const std::filesystem::path& dst, const std::filesystem::path& src);


	/*
		returns a string explaination of a file action enum
	*/
	std::string action_to_string(core::file_action action);


	/*
		returns string information on file_entry object entry formats it for output
	*/
	std::string output_file_entry(const core::file_entry& entry);


	/*
		returns a string of information on the file_type enum type
	*/
	std::string file_type_to_string(std::filesystem::file_type type);
	

	/*
		sends a file system error code to a string
	*/
	std::string output_filesystem_ec(std::error_code ec);

	
	/*
		returns a vector of specified entry type
		for example: if you wanted only entries with copy argument
		you would pass in args::copy as specific_arg and any entries 
		in dirt list that have copy as an argument will get returned in 
		the returned vector
	*/
	std::vector<core::arg_entry> get_specific_entries(const std::vector<core::arg_entry>& v,core::args specific_arg);


	/*
		returns total file size in bytes from a given directory path
	*/
	std::uintmax_t total_size(const std::filesystem::path& p);


	/*
		splits a queue into smaller queues

		number_of_qs is the number of queues to create from buffer_q
	*/
	std::vector<std::queue<core::file_entry>> split_queue(std::queue<core::file_entry>& buffer_q, std::size_t number_of_qs);
	
	
	/*
		returns the window width in pixels, throws an exception on error
	*/
	UINT get_window_width(HWND window_handle);


	/*
		returns the windows height in pixels, throws an exception on error
	*/
	UINT get_window_height(HWND window_handle);


	/*
		returns the window height, uses an error code for error handling
	*/
	UINT get_window_height(HWND window_handle, core::codes* code) noexcept;


	/*
		returns the window width in pixels, uses an error code for error handling
	*/
	UINT get_window_width(HWND window_handle, core::codes* code) noexcept;


	/* 	
		String conversions:
		
		if max_string_buffer is exceeded new is used to allocate a heap buffer to compensate
		throws an exception on error.
	*/ 
	const size_t max_string_buffer = MAX_STRING;
	std::wstring to_wide_string(const char* narrow);
	std::wstring to_wide_string(const std::string& narrow);
	std::string to_narrow_string(const wchar_t* wide);
	std::string to_narrow_string(const std::wstring& wide);
	
	
	
	/*
	 	String conversions:
		
		with error codes instead of exceptions, if max_string_buffer is exceeded
		new is used to allocate a heap buffer to compensate. Uses error codes returned via 
		pointer.
	*/	
	std::wstring to_wide_string(const char* narrow, core::codes* code_p);
	std::wstring to_wide_string(const std::string& narrow, core::codes* code_p);
	std::string to_narrow_string(const wchar_t* wide, core::codes* code_p);
	std::string to_narrow_string(const std::wstring& wide, core::codes* code_p);


	/*
		cmd line parsing
	*/
	std::vector<core::arg_entry> cmd_line(int argc, char* argv[], core::codes* code);
	core::codes validate(std::vector<core::arg_entry>& v);
	std::vector<core::arg_entry> cmd_line(const std::vector<std::string>& v, core::codes* code);

	/*
		time
	*/
	std::string terminal_time_now(const std::string& message);
	std::string time_to_string(const std::chrono::system_clock::time_point& time);
	std::string time_now(const std::string& message);

}