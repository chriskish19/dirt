#include "dt_api.hpp"

/**********************************************************/
//
// File: dt_api.cpp
//
// Purpose: dt_api.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_API_INCLUDE_PATH

#if ENABLE_API_LOGS
namespace api {
	std::unique_ptr<core::backend::system_log> logger = nullptr;
}
#endif



std::vector<std::string> api::convert_cmdline_args_to_utf8(LPWSTR* wide_argv, int argc)
{
	std::vector<std::string> narrow_args;
	if (wide_argv == nullptr) {
		return {};
	}


	for (int i = 0; i < argc; ++i)
	{
		narrow_args.push_back(api::to_narrow_string(wide_argv[i]));
	}

	LocalFree(wide_argv);
	return narrow_args;
}

std::string api::terminal_time_to_string(const std::chrono::system_clock::time_point& time)
{
	try {
		std::string time = std::format("{}[{}]{}", BRIGHT_GREEN, time , DEFAULT_COLOR);
		return time;
	}
	catch (const std::exception& e) {
		output_em(core::exception_thrown_and_handled_pkg);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	// exception thrown we return nothing
	return {};
}

core::arg_pkg api::match_arg_enum(core::args arg)
{
	switch (arg) {
	case core::args::copy:
		return core::copy_pkg;
	case core::args::dirt_list_path:
		return core::dirt_list_path_pkg;
	case core::args::mirror:
		return core::mirror_pkg;
	case core::args::new_files_only:
		return core::new_files_only_pkg;
	case core::args::no_deletes:
		return core::no_deletes_pkg;
	case core::args::uninit:
		return core::uninit_pkg;
	case core::args::watch:
		return core::watch_pkg;
	case core::args::src:
		return core::src_pkg;
	case core::args::dst:
		return core::dst_pkg;
	case core::args::recursive:
		return core::recursive_pkg;
	default:
		return core::unknown_pkg;
	}
}

core::arg_pkg api::match_s_arg(const std::string& arg)
{
	if (arg == core::copy_pkg.m_s_arg) {
		return core::copy_pkg;
	}
	if (arg == core::dirt_list_path_pkg.m_s_arg) {
		return core::dirt_list_path_pkg;
	}
	if (arg == core::mirror_pkg.m_s_arg) {
		return core::mirror_pkg;
	}
	if (arg == core::new_files_only_pkg.m_s_arg) {
		return core::new_files_only_pkg;
	}
	if (arg == core::no_deletes_pkg.m_s_arg) {
		return core::no_deletes_pkg;
	}
	if (arg == core::uninit_pkg.m_s_arg) {
		return core::uninit_pkg;
	}
	if (arg == core::watch_pkg.m_s_arg) {
		return core::watch_pkg;
	}
	if (arg == core::src_pkg.m_s_arg) {
		return core::src_pkg;
	}
	if (arg == core::dst_pkg.m_s_arg) {
		return core::dst_pkg;
	}
	if (arg == core::recursive_pkg.m_s_arg) {
		return core::recursive_pkg;
	}
	return core::unknown_pkg;
}

core::code_pkg api::match_code(core::codes code)
{
	switch (code) {
		// basic
	case core::codes::success:
		return core::success_pkg;
	case core::codes::uninit:
		return core::c_uninit_pkg;

		// parsing dt file
	case core::codes::syntax_mistake:
		return core::syntax_mistake_pkg;
	case core::codes::invalid_cmd_args:
		return core::invalid_cmd_args_pkg;
	case core::codes::no_valid_entries:
		return core::no_valid_entries_pkg;

		// general
	case core::codes::pointer_is_null:
		return core::pointer_is_null_pkg;
	case core::codes::to_wide_string_failed:
		return core::to_wide_string_failed_pkg;
	case core::codes::to_narrow_string_failed:
		return core::to_narrow_string_failed_pkg;

		// exceptions
	case core::codes::exception_thrown_and_handled:
		return core::exception_thrown_and_handled_pkg;
	case core::codes::std_filesystem_exception_caught:
		return core::std_filesystem_exception_caught_pkg;
	case core::codes::unknown_exception_caught:
		return core::unknown_exception_caught_pkg;

		// file related
	case core::codes::file_copy_fail:
		return core::file_copy_fail_pkg;
	case core::codes::file_delete_fail:
		return core::file_delete_fail_pkg;
	case core::codes::directory_delete_fail:
		return core::directory_delete_fail_pkg;
	case core::codes::invalid_directory_path:
		return core::invalid_directory_path_pkg;
	case core::codes::file_open_fail:
		return core::file_open_fail_pkg;
	case core::codes::invalid_file_path:
		return core::invalid_file_path_pkg;
	case core::codes::write_to_file_fail:
		return core::write_to_file_fail_pkg;

		// win32 codes
	case core::codes::failed_to_register_class:
		return core::failed_to_register_class_pkg;
	case core::codes::hwnd_fail:
		return core::hwnd_fail_pkg;
	case core::codes::show_window_fail:
		return core::show_window_fail_pkg;
	case core::codes::getclientrect_fail:
		return core::getclientrect_fail_pkg;
	case core::codes::invalid_io_handle:
		return core::invalid_io_handle_pkg;
	case core::codes::handle_nullptr:
		return core::handle_nullptr_pkg;
	case core::codes::read_dir_changes_fail:
		return core::read_dir_changes_fail_pkg;
	case core::codes::get_text_metrics_fail:
		return core::get_text_metrics_fail_pkg;
	
	default:
		return core::c_unknown_pkg;
	}
}

void api::output_em(const core::code_pkg cp, const std::string location) {
	std::string output = std::format("{}\n{}\n", cp.message(), location);


#if ENABLE_API_LOGS
	api::logger->log_message(output);
#endif


#if WIDE
	core::codes code;
	OutputDebugString(api::to_wide_string(output, &code).c_str());
	if (code != core::codes::success) {
		std::wstring w_output = std::format(
			L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
			__FILEW__,
			__LINE__
		);
		OutputDebugString(w_output.c_str());
	}
#else
	OutputDebugString(output.c_str());
#endif
}

void api::output_fse(const std::filesystem::filesystem_error& e)
{
	std::string output = std::format(
		"Message: {}\n Path 1: {}\n Path 2: {}\n",
		e.what(),
		e.path1().string(),
		e.path2().string()
	);

#if ENABLE_API_LOGS
	api::logger->log_message(output);
#endif

#if WIDE
	core::codes code;
	OutputDebugString(api::to_wide_string(output, &code).c_str());
	if (code != core::codes::success) {
		std::wstring w_output = std::format(
			L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
			__FILEW__,
			__LINE__
		);
		OutputDebugString(w_output.c_str());
	}
#else
	OutputDebugString(output.c_str());
#endif
}

std::string api::get_location(std::source_location sl)
{
	std::string function_name = sl.function_name();
	std::string line = std::to_string(sl.line());
	std::string column = std::to_string(sl.column());
	std::string file_name = sl.file_name();

	std::string temp = std::format("File name: {} \n - column: {} \n - line: {} \n - function name: {} \n", file_name, column, line, function_name);
	return temp;
}

std::vector<core::args> api::validate_args(const std::vector<core::arg_pkg>& args)
{
	std::vector<core::args> arg_v;
	for (auto& arg : args) {
		arg_v.push_back(arg.m_arg);
	}

	core::group_2 g2;
	core::group_3 g3;
	core::group_4 g4;
	core::group_5 g5;

	if (g2.match_group(arg_v) == true or 
		g3.match_group(arg_v) == true or 
		g4.match_group(arg_v) == true or
		g5.match_group(arg_v) == true)
	{
		return arg_v;
	}
	else {
		// empty vector no match group found
		return {};
	}
}

std::vector<core::arg_entry> api::parse_file(const std::filesystem::path& p, core::codes* code_p)
{
	// file object stream
	std::ifstream file;
	
	// try to open the file from the path p
	file.open(p);
	
	// if it fails to open we return an empty vector and set 
	// the proper value for the error code
	if (file.is_open() == false) {
		*code_p = core::codes::file_open_fail;
		return {};
	}
	
	// see CORE_ARGS_INCLUDE_PATH for definition.
	// each listed entry in the file needs an object 
	// placeholder then it gets added to the vector.
	core::arg_entry entry;	
	
	// the return vector
	std::vector<core::arg_entry> entry_v;

	// a line of text in the file
	std::string line;
	
	// numbered from top to bottom starts at zero
	std::size_t entry_number = 0;
	
	// a placeholder for the previous word
	std::string last_word;
	
	// main loop that goes through the file line by line starting at the top
	// each time std::getline is called the variable line is modified to the text at next line
	while (std::getline(file, line)) {
		
		// ignore blank lines
		if (line.empty() == true) {
			continue;
		}

		// ignore comments
		std::size_t comment_slash_pos = line.find_first_of("//");
		std::size_t comment_start_pos = line.find_first_not_of("//");
		std::size_t comment_end_pos = line.find_first_of("\n");
		if (comment_start_pos != std::string::npos 
			and comment_slash_pos != std::string::npos
			and comment_end_pos != std::string::npos) {
			// we have a comment so skip the line
			continue;
		}
		
		// separete the words on each line
		std::string word;					// each word
		std::istringstream iss(line);		// string stream the line
		
		// loop through the line word by word (operator >> uses spaces to separete words)
		while (iss >> word) {
			
			// look for matching words in the global args map
			auto found = core::gbl_args_mp.find(word);
			
			/*
				this if statement block is for setting word and last words values
			*/
			// if a word is matched we add it to the entry args vector
			if (found != core::gbl_args_mp.end()) {
				entry.args_v.push_back(found->second);	// found->second is the enum argument
			}
			else if(word == "args"){ 					// word is args so we continue
				last_word = word;						// set the previous word to word
				continue;								// skip to begin loop run and on to the next word
			}
			else if (word == "{") {						// if open curly for word 
				// the next word should be src
				last_word = word;						// set it to last word
				continue;								// skip to begin loop run and on to the next word
			}
			

			/*
				this if statement block is for grabbing all the arguments (-copy -recursive etc...) 
			*/
			if (last_word == "args" and word == ":") {
				// the next words should be arguments
				while (iss >> word) {
					// search the map for the arguments
					auto found = core::gbl_args_mp.find(word);
					if (found != core::gbl_args_mp.end()) {				// if found add them to the vector
						entry.args_v.push_back(found->second);
					}
				}
				
			}
			
			
			/*
				this if statement block is for grabbing the src path
			*/
			if (last_word == "{" and word == "src") {
				// the next should be the src path
				iss >> std::quoted(word);
				entry.src_p = word;
			}
		
			/*
				this if statement block is for grabbing the dst path
			*/
			if (word == "dst") {
				// the next should be the dst path
				iss >> std::quoted(word);
				entry.dst_p = word;
			}
		}
		
		/*
			this if statement block is the end of each entry
		*/ 
		if (line.find_first_of("}") != std::string::npos) {
			entry_number++;
			entry.entry_number = entry_number;
			
			// end of entry
			entry_v.push_back(entry);
			entry = core::arg_entry(); // reset
		}
	}
	
	// we successfully read the file and got the entries
	*code_p = core::codes::success;
	return entry_v;
}

std::vector<core::args> api::arg_pkg_to_args(const std::vector<core::arg_pkg>& args_pkg_v)
{
	std::vector<core::args> args_v;
	for (auto& arg : args_pkg_v) {
		args_v.push_back(arg.m_arg);
	}
	return args_v;
}

bool api::validate_entry(const core::arg_entry& e)
{
	
	core::group_2 g2;
	core::group_3 g3;
	core::group_4 g4;
	core::group_5 g5;

	if (g2.match_group(e.args_v) == true or
		g3.match_group(e.args_v) == true or
		g4.match_group(e.args_v) == true or
		g5.match_group(e.args_v) == true)
	{}
	else {
		return false;
	}

	return	std::filesystem::is_directory(e.src_p) and
			std::filesystem::is_directory(e.dst_p);
}

std::string api::get_last_error_w32()
{
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0) {
		return {}; // No error message has been recorded 
	}

	core::character_p messageBuffer = nullptr;

	// Format the error message 
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(core::character_p)&messageBuffer,
		0,
		NULL
	);

	// Copy the error message into a core::string
	core::string message(messageBuffer, size);

	// Free the buffer allocated by FormatMessage 
	LocalFree(messageBuffer);

#if WIDE
	// this will throw if an error occurs while converting to narrow string, keep that in mind
	return to_narrow_string(message);
#else
	return message;
#endif
}

std::string api::output_entry(const core::arg_entry& e)
{
	// holds the matched arg to its string value
	std::vector<std::string> s_v;
	
	// for each argument in the entry vector
	// match its string value and add it to s_v vector
	for (auto arg : e.args_v) {
		auto pkg = match_arg_enum(arg);
		s_v.push_back(pkg.m_s_arg);
	}

	// outputing to the console:
	std::string message;
	
	// list each argument and add it to the message
	message = "Entry number: " + std::to_string(e.entry_number) + '\n';
	for (const auto& s : s_v) {
		message += "arg: " + s + '\n';
	}
	
	// add the dst path and src path to the message
	message += "Destination Path: " + e.dst_p.string() + '\n'
	+ "Source Path: " + e.src_p.string() + '\n';
	
	return message;
}

std::string api::text_output_fse(const std::filesystem::filesystem_error& e)
{
	// create the string error message
	std::string message = "Message: " + std::string(e.what()) + '\n' 
		+ "Path 1: " + e.path1().string() + '\n'
		+ "Path 2: " + e.path2().string() + '\n';
	
	return message;
}

std::uintmax_t api::file_numbers(const std::filesystem::path& p)
{
	// returns 0 on a number of problems, if it does not exist and its not a directory 
	// it will return 0 and no errors will be thrown.
	// only counts regular files, nothing else.
	try {
		std::uintmax_t count = 0;
		if (std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
			for (const auto& entry : std::filesystem::directory_iterator(p)) {
				if (entry.is_regular_file()) {
					++count;
				}
			}
		}
		return count;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(core::std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	
	// exception thrown, error occured so return 0
	return 0;
}

std::unordered_set<core::directory_info> api::get_all_directories(const std::filesystem::path& p)
{
	try {
		// if it doesnt exist or its not directory, return a blank set
		if (std::filesystem::exists(p) == false or std::filesystem::is_directory(p) == false) {
			return {}; // empty set 
		}
		
		// the returned set of directory entries
		std::unordered_set<core::directory_info> di_set;
		
		// recursively loop through each directory  
		for (const auto& entry : std::filesystem::recursive_directory_iterator(p)) {
			if (entry.is_directory() == true) {
				core::directory_info di;
				di.p = entry.path();
				di.number_of_files = file_numbers(entry.path());
				di_set.emplace(di);
			}
		}
		
		// the set is filled up so return it
		return di_set;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(core::std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	
	// an exception was thrown, lets return a blank set
	return {};
}

core::codes api::copy_directory_only(const std::filesystem::path& dst, const std::filesystem::path& src)
{
	try {
		// checks if src and dst are directories
		if (std::filesystem::is_directory(dst) == false or std::filesystem::is_directory(src) == false) {
			// if they are not directories return an error code
			return core::codes::invalid_directory_path;
		}
		
		// copy src to dst
		std::filesystem::copy(src, dst, std::filesystem::copy_options::update_existing);
		
		// success
		return core::codes::success;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(core::std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	
	// an exception was thrown, return the code
	return core::codes::exception_thrown_and_handled;
}

core::codes api::copy_directory_recursive(const std::filesystem::path& dst, const std::filesystem::path& src)
{
	try {
		if (std::filesystem::is_directory(dst) == false or std::filesystem::is_directory(src) == false) {
			return core::codes::invalid_directory_path;
		}

		std::filesystem::copy(src, dst, 
			std::filesystem::copy_options::update_existing |
			std::filesystem::copy_options::recursive);

		return core::codes::success;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(core::std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	return core::codes::exception_thrown_and_handled;
}

std::string api::action_to_string(core::file_action action)
{
	switch (action) {
	case core::file_action::copy:
		return "copy the file from the source directory to the destination directory";
	case core::file_action::delete_dst:
		return "delete the file in the destination directory";
	case core::file_action::modified:
		return "file has been modified";
	case core::file_action::new_name:
		return "file has been renamed, this is the new name";
	case core::file_action::previous_name:
		return "file has been renamed, this is the old name";
	case core::file_action::uninit:
		return "the file action enum is uninitialized, no value has been set";
	default:
		return "no matching file action enum found";
	}
}

std::string api::file_type_to_string(std::filesystem::file_type type) {
	switch (type) {
	case std::filesystem::file_type::none:
		return "No file type information available.";
	case std::filesystem::file_type::not_found:
		return "The file does not exist.";
	case std::filesystem::file_type::regular:
		return "A regular file (like a text or binary file).";
	case std::filesystem::file_type::directory:
		return "A directory or folder.";
	case std::filesystem::file_type::symlink:
		return "A symbolic link (symlink).";
	case std::filesystem::file_type::block:
		return "A block device (like a hard drive or disk partition).";
	case std::filesystem::file_type::character:
		return "A character device (like a serial port or terminal).";
	case std::filesystem::file_type::fifo:
		return "A FIFO (named pipe).";
	case std::filesystem::file_type::socket:
		return "A Unix domain socket.";
	case std::filesystem::file_type::unknown:
		return "The file type is unknown.";
	default:
		return "Unrecognized or unsupported file type.";
	}
}

std::string api::output_filesystem_ec(std::error_code ec)
{
	return "filesystem error (" + std::to_string(ec.value()) + "): " + ec.message() + '\n';
}

std::vector<core::arg_entry> api::get_specific_entries(const std::vector<core::arg_entry>& v, core::args specific_arg)
{
	std::vector<core::arg_entry> watch_entries_v;
	for (const auto& e : v) {
		auto found = std::find(e.args_v.begin(), e.args_v.end(), specific_arg);
		if (found != e.args_v.end()) {
			watch_entries_v.push_back(e);
		}
	}
	return watch_entries_v;
}

std::uintmax_t api::total_size(const std::filesystem::path& p)
{
	try {
		std::uintmax_t total_file_size = 0;
		if (std::filesystem::exists(p) == true && std::filesystem::is_directory(p) == true) {
			for (const auto& entry : std::filesystem::recursive_directory_iterator(p)) {
				total_file_size += entry.file_size();
			}
		}
		return total_file_size;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(core::std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	return 0;
}

std::vector<std::queue<core::file_entry>> api::split_queue(std::queue<core::file_entry>& buffer_q, std::size_t number_of_qs)
{
	std::vector<std::queue<core::file_entry>> file_entry_v_q;
	auto buffer_q_size = buffer_q.size();

	if (buffer_q_size == 0) {
		return {};
	}

	auto q_size = buffer_q_size / number_of_qs;

	while (buffer_q.empty() == false) {
		std::queue<core::file_entry> q;
		while (q.size() < q_size and buffer_q.empty() == false) {
			core::file_entry entry = buffer_q.front();
			q.emplace(entry);
			buffer_q.pop();
		}
		file_entry_v_q.push_back(q);
	}
	return file_entry_v_q;
}


std::string api::output_file_entry(const core::file_entry& entry)
{
	std::string text = std::format(
		"\n Source Path: {}\n Destination Path: {}\n Action: {}\n File type: {}\n",
		entry.src_p.string(),
		entry.dst_p.string(),
		action_to_string(entry.action),
		file_type_to_string(entry.src_s.type())
	);

	return text;
}

UINT api::get_window_width(HWND window_handle)
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        throw core::getclientrect_fail_pkg;
    }

    return rc.right - rc.left;
}

UINT api::get_window_height(HWND window_handle) {
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        throw core::getclientrect_fail_pkg;
    }

    return rc.bottom - rc.top;
}

UINT api::get_window_width(HWND window_handle,core::codes* code) noexcept
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        *code = core::codes::getclientrect_fail;
        return 0;
    }

    return rc.right - rc.left;
}

UINT api::get_window_height(HWND window_handle, core::codes* code) noexcept
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        *code = core::codes::getclientrect_fail;
        return 0;
    }

    return rc.bottom - rc.top;
}

std::wstring api::to_wide_string(const char* narrow) {
    /*

    std::size_t mbsrtowcs( wchar_t* dst,
                       const char** src,
                       std::size_t len,
                       std::mbstate_t* ps );

    dst 	- 	pointer to wide character array where the results will be stored
    src 	- 	pointer to pointer to the first element of a null-terminated multibyte string
    len 	- 	number of wide characters available in the array pointed to by dst
    ps 	- 	pointer to the conversion state object

    */

    // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    // get the length in bytes of "temp"
    std::size_t length = 1 + std::mbsrtowcs(nullptr, &narrow, 0, &state);

    // stack buffer
    wchar_t* buffer = nullptr;
    wchar_t stack_buffer[max_string_buffer];
    bool heap_allocated = false;

    // if length is greater than max_string_buffer, we will allocate a new buffer using new
    if (length > max_string_buffer) {
        buffer = new wchar_t[length];
        heap_allocated = true;
    }
    else {
        buffer = stack_buffer;
    }


    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::mbsrtowcs(buffer, &narrow, length, &state);

    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        // clean up first
        if (heap_allocated == true and buffer != nullptr) {
            delete[] buffer;
        }

        // now we throw an exception after clean up
		std::string location = get_location();
        throw core::dtapierror(core::to_wide_string_failed_pkg,location,"");

        // returns an empty string
        return {};
    }

    // clean up
    if (heap_allocated == true and buffer != nullptr) {
        std::wstring r_temp(buffer);
        delete[] buffer;
        return r_temp;
    }

    // return the wide string using the stack buffer
    return std::wstring(buffer);
}

std::wstring api::to_wide_string(const std::string& narrow) {
    /*

    std::size_t mbsrtowcs( wchar_t* dst,
                       const char** src,
                       std::size_t len,
                       std::mbstate_t* ps );

    dst 	- 	pointer to wide character array where the results will be stored
    src 	- 	pointer to pointer to the first element of a null-terminated multibyte string
    len 	- 	number of wide characters available in the array pointed to by dst
    ps 	- 	pointer to the conversion state object

    */

    // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    // temp in order to get the address of the pointer
    const char* temp = narrow.c_str();

    // get the length in bytes of "temp"
    std::size_t length = 1 + std::mbsrtowcs(nullptr, &temp, 0, &state);

    // stack buffer
    wchar_t* buffer = nullptr;
    wchar_t stack_buffer[max_string_buffer];
    bool heap_allocated = false;

    // if length is greater than max_string_buffer, we will allocate a new buffer using new
    if (length > max_string_buffer) {
        buffer = new wchar_t[length];
        heap_allocated = true;
    }
    else {
        buffer = stack_buffer;
    }

    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::mbsrtowcs(buffer, &temp, length, &state);

    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        // clean up
        if (heap_allocated == true and buffer != nullptr) {
            delete[] buffer;
        }

        // now we throw an exception after clean up
		std::string location = get_location();
		throw core::dtapierror(core::to_wide_string_failed_pkg, location, "");

        // returns an empty string
        return {};
    }

    // clean up
    if (heap_allocated == true and buffer != nullptr) {
        std::wstring r_temp(buffer);
        delete[] buffer;
        return r_temp;
    }

    // return the wide string using the buffer
    return std::wstring(buffer);
}

std::string api::to_narrow_string(const wchar_t* wide) {
    /*
    *
    Converts a sequence of wide characters from the array whose first element is pointed to by *src
    to its narrow multibyte representation that begins in the conversion state described by *ps.
    If dst is not null, converted characters are stored in the successive elements of the char array
    pointed to by dst. No more than len bytes are written to the destination array.


    std::size_t wcsrtombs( char* dst,
                       const wchar_t** src,
                       std::size_t len,
                       std::mbstate_t* ps );

    dst 	- 	pointer to narrow character array where the multibyte characters will be stored
    src 	- 	pointer to pointer to the first element of a null-terminated wide string
    len 	- 	number of bytes available in the array pointed to by dst
    ps 	    - 	pointer to the conversion state object

    */

    // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    // get the wide string length, does not include '\0'
    // returns the length in bytes
    std::size_t length = 1 + std::wcsrtombs(nullptr, &wide, 0, &state);

    // stack buffer
    char* buffer = nullptr;
    char stack_buffer[max_string_buffer];
    bool heap_allocated = false;

    // if length is greater than max_string_buffer, we will allocate a new buffer using new
    if (length > max_string_buffer) {
        buffer = new char[length];
        heap_allocated = true;
    }
    else {
        buffer = stack_buffer;
    }

    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::wcsrtombs(buffer, &wide, length, &state);


    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {

        // clean up
        if (heap_allocated == true and buffer != nullptr) {
            delete[] buffer;
        }

        // throw an exception on error
		std::string location = get_location();
		throw core::dtapierror(core::to_narrow_string_failed_pkg, location, "");


        // returns an empty string
        return {};
    }

    // clean up
    if (heap_allocated == true and buffer != nullptr) {
        std::string r_temp(buffer);
        delete[] buffer;
        return r_temp;
    }

    // return the narrow string using the stack buffer
    return std::string(buffer);
}

std::string api::to_narrow_string(const std::wstring& wide) {
    /*
   *
   Converts a sequence of wide characters from the array whose first element is pointed to by *src
   to its narrow multibyte representation that begins in the conversion state described by *ps.
   If dst is not null, converted characters are stored in the successive elements of the char array
   pointed to by dst. No more than len bytes are written to the destination array.


   std::size_t wcsrtombs( char* dst,
                      const wchar_t** src,
                      std::size_t len,
                      std::mbstate_t* ps );

   dst 	- 	pointer to narrow character array where the multibyte characters will be stored
   src 	- 	pointer to pointer to the first element of a null-terminated wide string
   len 	- 	number of bytes available in the array pointed to by dst
   ps 	    - 	pointer to the conversion state object

   */

   // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    // get the wide string length, does not include '\0'
    // returns the length in bytes
    const wchar_t* temp = wide.c_str();
    std::size_t length = 1 + std::wcsrtombs(nullptr, &temp, 0, &state);

    // stack buffer
    char* buffer = nullptr;
    char stack_buffer[max_string_buffer];
    bool heap_allocated = false;

    // if length is greater than max_string_buffer, we will allocate a new buffer using new
    if (length > max_string_buffer) {
        buffer = new char[length];
        heap_allocated = true;
    }
    else {
        buffer = stack_buffer;
    }

    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */

    std::size_t error_code = std::wcsrtombs(buffer, &temp, length, &state);


    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {

        // clean up
        if (heap_allocated == true and buffer != nullptr) {
            delete[] buffer;
        }

        // throw an exception on error
		std::string location = get_location();
		throw core::dtapierror(core::to_narrow_string_failed_pkg, location, "");

        // returns an empty string
        return {};
    }

    // clean up
    if (heap_allocated == true and buffer != nullptr) {
        std::string r_temp(buffer);
        delete[] buffer;
        return r_temp;
    }

    // return the narrow string using the stack buffer
    return std::string(buffer);
}

std::wstring api::to_wide_string(const char* narrow, core::codes* code_p) {
    // return nothing if code is nullptr
    if (code_p == nullptr) {
        return {};
    }


    /*

    std::size_t mbsrtowcs( wchar_t* dst,
                       const char** src,
                       std::size_t len,
                       std::mbstate_t* ps );

    dst 	- 	pointer to wide character array where the results will be stored
    src 	- 	pointer to pointer to the first element of a null-terminated multibyte string
    len 	- 	number of wide characters available in the array pointed to by dst
    ps 	- 	pointer to the conversion state object

    */

    // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    /*

    On success, returns the number of wide characters, excluding the terminating L'\0',
    written to the character array. If dst is a null pointer, returns the number of wide
    characters that would have been written given unlimited length.

    */
    std::size_t length = 1 + std::mbsrtowcs(nullptr, &narrow, 0, &state);


    wchar_t* buffer = nullptr;

    // stack buffer
    wchar_t stack_buffer[max_string_buffer];
    buffer = stack_buffer;
    bool heap_alloc = false;

    // if length is greater than max_string_buffer, we allocate memory:
    if (length > max_string_buffer) {
        buffer = new wchar_t[length];
        heap_alloc = true;
    }


    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::mbsrtowcs(buffer, &narrow, length, &state);

    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        *code_p = core::codes::to_wide_string_failed;

        if (heap_alloc == true and buffer != nullptr) {
            delete[] buffer;
        }

        // returns an empty string
        return {};
    }

    std::wstring temp_buffer(buffer);

    if (heap_alloc == true and buffer != nullptr) {
        delete[] buffer;
    }

	*code_p = core::codes::success;

    // return the wide string using the buffer
    return temp_buffer;
}

std::wstring api::to_wide_string(const std::string& narrow, core::codes* code_p) {
    // return nothing if code is nullptr
    if (code_p == nullptr) {
        return {};
    }


    /*

    std::size_t mbsrtowcs( wchar_t* dst,
                       const char** src,
                       std::size_t len,
                       std::mbstate_t* ps );

    dst 	- 	pointer to wide character array where the results will be stored
    src 	- 	pointer to pointer to the first element of a null-terminated multibyte string
    len 	- 	number of wide characters available in the array pointed to by dst
    ps 	- 	pointer to the conversion state object

    */

    // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    const char* temp = narrow.c_str();

    /*

    On success, returns the number of wide characters, excluding the terminating L'\0',
    written to the character array. If dst is a null pointer, returns the number of wide
    characters that would have been written given unlimited length.

    */
    std::size_t length = 1 + std::mbsrtowcs(nullptr, &temp, 0, &state);


    wchar_t* buffer = nullptr;

    // stack buffer
    wchar_t stack_buffer[max_string_buffer];
    buffer = stack_buffer;
    bool heap_alloc = false;

    // if length is greater than max_string_buffer, allocate more memory.
    if (length > max_string_buffer) {
        buffer = new wchar_t[length];
        heap_alloc = true;
    }


    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::mbsrtowcs(buffer, &temp, length, &state);

    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        *code_p = core::codes::to_wide_string_failed;

        if (heap_alloc and buffer != nullptr) {
            delete[] buffer;
        }

        // returns an empty string
        return {};
    }

    std::wstring temp_buffer(buffer);

    if (heap_alloc and buffer != nullptr) {
        delete[] buffer;
    }

    *code_p = core::codes::success;

    // return the wide string using the buffer
    return temp_buffer;
}

std::string api::to_narrow_string(const wchar_t* wide, core::codes* code_p) {
    // return nothing if code is nullptr
    if (code_p == nullptr) {
        return {};
    }


    /*
   *
   Converts a sequence of wide characters from the array whose first element is pointed to by *src
   to its narrow multibyte representation that begins in the conversion state described by *ps.
   If dst is not null, converted characters are stored in the successive elements of the char array
   pointed to by dst. No more than len bytes are written to the destination array.


   std::size_t wcsrtombs( char* dst,
                      const wchar_t** src,
                      std::size_t len,
                      std::mbstate_t* ps );

   dst 	- 	pointer to narrow character array where the multibyte characters will be stored
   src 	- 	pointer to pointer to the first element of a null-terminated wide string
   len 	- 	number of bytes available in the array pointed to by dst
   ps 	    - 	pointer to the conversion state object

   */

   // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    // get the wide string length, does not include '\0'
    // returns the length in bytes
    std::size_t length = 1 + std::wcsrtombs(nullptr, &wide, 0, &state);

    char* buffer = nullptr;
    char temp_buffer[max_string_buffer];
    bool heap_alloc = false;


    // if length is greater than max_string_buffer, we have an error:
    // use new to allocate memory to compensate 
    if (length > max_string_buffer){
        buffer = new char[length];
        heap_alloc = true;
    }
    else {
        buffer = temp_buffer;
    }


    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::wcsrtombs(buffer, &wide, length, &state);


    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        *code_p = core::codes::to_narrow_string_failed;

        if (heap_alloc and buffer != nullptr) {
            delete[] buffer;
        }

        // returns an empty string
        return {};
    }

    std::string temp_buffer_str(buffer);

    if (heap_alloc and buffer != nullptr) {
        delete[] buffer;
    }

    *code_p = core::codes::success;

    // return the narrow string using the buffer
    return temp_buffer_str;
}

std::string api::to_narrow_string(const std::wstring& wide, core::codes* code_p) {
    // return nothing if code is nullptr
    if (code_p == nullptr) {
        return {};
    }


    /*
   *
   Converts a sequence of wide characters from the array whose first element is pointed to by *src
   to its narrow multibyte representation that begins in the conversion state described by *ps.
   If dst is not null, converted characters are stored in the successive elements of the char array
   pointed to by dst. No more than len bytes are written to the destination array.


   std::size_t wcsrtombs( char* dst,
                      const wchar_t** src,
                      std::size_t len,
                      std::mbstate_t* ps );

   dst 	- 	pointer to narrow character array where the multibyte characters will be stored
   src 	- 	pointer to pointer to the first element of a null-terminated wide string
   len 	- 	number of bytes available in the array pointed to by dst
   ps 	    - 	pointer to the conversion state object

   */

   // Create a fresh conversion state per thread
    std::mbstate_t state = std::mbstate_t();

    const wchar_t* temp = wide.c_str();

    // get the wide string length, does not include '\0'
    // returns the length in bytes
    std::size_t length = 1 + std::wcsrtombs(nullptr, &temp, 0, &state);

    char* buffer = nullptr;
    char temp_buffer[max_string_buffer];
    bool heap_alloc = false;


    // if length is greater than max_string_buffer, we have an error:
    // use new to allocate memory to compensate 
    if (length > max_string_buffer) {
        buffer = new char[length];
        heap_alloc = true;
    }
    else {
        buffer = temp_buffer;
    }


    // according to documentation:
    /*

    The following functions should not be called from multiple threads without synchronization with
    the std::mbstate_t* argument of a null pointer due to possible data races: std::mbrlen, std::mbrtowc,
    std::mbsrtowcs, std::mbtowc, std::wcrtomb, std::wcsrtombs, std::wctomb.

    */
    std::size_t error_code = std::wcsrtombs(buffer, &temp, length, &state);


    // On conversion error (if invalid wide character was encountered), 
    // returns static_cast<std::size_t>(-1), stores EILSEQ in errno, and leaves *ps in unspecified state. 
    if (error_code == std::size_t(-1)) {
        *code_p = core::codes::to_narrow_string_failed;

        if (heap_alloc and buffer != nullptr) {
            delete[] buffer;
        }

        // returns an empty string
        return {};
    }

    std::string temp_buffer_str(buffer);

    if (heap_alloc and buffer != nullptr) {
        delete[] buffer;
    }

    *code_p = core::codes::success;

    // return the narrow string using the buffer
    return temp_buffer_str;
}

std::vector<core::arg_entry> api::cmd_line(int argc, char* argv[], core::codes* code)
{	
	std::vector<core::arg_entry> entry_v;

	bool next_is_dtlist = false;
	bool using_dtlist = false;

	std::filesystem::path dtlist_path;

	core::args last_arg = core::args();

	core::arg_entry single_entry;

	for (int i = 0; i < argc; ++i) {
		std::string word = argv[i];
		
		if (last_arg == core::args::src) {
			single_entry.src_p = word;
		}
		else if (last_arg == core::args::dst) {
			single_entry.dst_p = word;
		}

		if (next_is_dtlist == true) {
			dtlist_path = word;
			using_dtlist = true;
			break;
		}

		auto found = core::gbl_args_mp.find(word);
		if (found != core::gbl_args_mp.end()) {
			if (found->second == core::args::dirt_list_path) {
				next_is_dtlist = true;
			}
			else {
				single_entry.args_v.push_back(found->second);
				last_arg = found->second;
			}
		}
	}

	if (using_dtlist == true) {
		entry_v = api::parse_file(dtlist_path,code);
		return entry_v;
	}
	else { // using one entry cmdline
		single_entry.entry_number = 1;
		entry_v.push_back(single_entry);
		*code = core::codes::success;
		return entry_v;
	}
	return {};
}

core::codes api::validate(std::vector<core::arg_entry>& v)
{
	std::vector<std::size_t> invalid_entrys_v;
	for (const auto& entry : v) {
		if (validate_entry(entry) == false) {
#if ENABLE_API_LOGS
			api::logger->log_message("error : invalid entry\n");
			api::logger->log_message(output_entry(entry));
#endif
			output_message("Error : Invalid entry\n");
			output_message(output_entry(entry));

			invalid_entrys_v.push_back(entry.entry_number);
		}
	}

	for (auto i : invalid_entrys_v) {
		v.erase(v.begin() + (i - 1));
	}

	if (v.empty()) {
		return core::codes::no_valid_entries;
	}
	return core::codes::success;
}

std::vector<core::arg_entry> api::cmd_line(const std::vector<std::string>& v, core::codes* code)
{
	std::vector<core::arg_entry> entry_v;

	bool next_is_dtlist = false;
	bool using_dtlist = false;

	std::filesystem::path dtlist_path;

	core::args last_arg = core::args();

	core::arg_entry single_entry;

	for (const auto& word : v) {
		
		if (last_arg == core::args::src) {
			single_entry.src_p = word;
		}
		else if (last_arg == core::args::dst) {
			single_entry.dst_p = word;
		}

		if (next_is_dtlist == true) {
			dtlist_path = word;
			using_dtlist = true;
			break;
		}

		auto found = core::gbl_args_mp.find(word);
		if (found != core::gbl_args_mp.end()) {
			if (found->second == core::args::dirt_list_path) {
				next_is_dtlist = true;
			}
			else {
				single_entry.args_v.push_back(found->second);
				last_arg = found->second;
			}
		}
	}

	if (using_dtlist == true) {
		entry_v = api::parse_file(dtlist_path, code);
		return entry_v;
	}
	else { // using one entry cmdline
		single_entry.entry_number = 1;
		entry_v.push_back(single_entry);
		*code = core::codes::success;
		return entry_v;
	}
	return {};
}

std::vector<core::arg_entry> api::windows_cmd_line(core::codes* code)
{
	int argc = 0;
	LPWSTR* wide_argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	if (wide_argv == nullptr)
	{
		api::output_cp(core::pointer_is_null_pkg);
		*code = core::codes::pointer_is_null;
		return {};
	}


	std::vector<std::string> argsv = api::convert_cmdline_args_to_utf8(wide_argv, argc);
	std::vector<core::arg_entry> entry_v;
	
	{
		core::codes local_code;
		entry_v = api::cmd_line(argsv, &local_code);
		*code = local_code;
		if (local_code != core::codes::success) {
			auto pkg = api::match_code(local_code);
			api::output_cp(pkg);
		}
	}

	{
		core::codes local_code = api::validate(entry_v);
		*code = local_code;
		if (local_code != core::codes::success) {
			auto pkg = api::match_code(local_code);
			api::output_cp(pkg);
		}
	}

	return entry_v;
}

std::string api::terminal_time_now(const std::string& message)
{
	try {
		auto now = std::chrono::system_clock::now();
		// time is green in terminal
		std::string time = std::format("{}[{}]{}", BRIGHT_GREEN , now, DEFAULT_COLOR);
		return time + message;
	}
	catch (const std::exception& e) {
		output_em(core::exception_thrown_and_handled_pkg);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	// exception thrown we return nothing
	return {};
}

std::string api::time_now(const std::string& message) {
	try {
		auto now = std::chrono::system_clock::now();
		std::string time = std::format("[{}]", now);
		return time + message;
	}
	catch (const std::exception& e) {
		output_em(core::exception_thrown_and_handled_pkg);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	// exception thrown we return nothing
	return {};
}

std::string api::time_to_string(const std::chrono::system_clock::time_point& time)
{
	try {
		std::string stime = std::format("[{}]", time);
		return stime;
	}
	catch (const std::exception& e) {
		output_em(core::exception_thrown_and_handled_pkg);
	}
	catch (...) {
		output_em(core::unknown_exception_caught_pkg);
	}
	// exception thrown we return nothing
	return {};
}

void api::output_le(const core::le& e)
{
	std::string output = std::format(
		"DESCRIPTION: {}\n WINDOWS ERROR: {}\n LOCATION: {}\n",
		e.message(),
		e.windows_error(),
		e.location()
		);
#if WIDE
	core::codes code;
	OutputDebugString(api::to_wide_string(output, &code).c_str());
	if (code != core::codes::success) {
		std::wstring w_output = std::format(
			L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
			__FILEW__,
			__LINE__
		);
		OutputDebugString(w_output.c_str());
	}
#else
	OutputDebugString(output.c_str());
#endif
}

void api::output_cp(const core::code_pkg& cp, const std::string& location)
{
	std::string output = std::format(
		"DESCRIPTION: {}\n LOCATION: {}\n",
		cp.message(),
		location
	);
#if WIDE
	{
		core::codes code;
		OutputDebugString(api::to_wide_string(output, &code).c_str());
		if (code != core::codes::success) {
			std::wstring w_output = std::format(
				L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
				__FILEW__,
				__LINE__
			);
			OutputDebugString(w_output.c_str());
		}
	}
#else
	OutputDebugString(output.c_str());
#endif
}

void api::output_dtapierror(const core::dtapierror& e)
{
	std::string output = std::format(
		"DESCRIPTION: {}\n WINDOWS ERROR: {}\n LOCATION: {}\n",
		e.message(),
		e.windows_error(),
		e.location()
	);

#if WIDE
	{
		core::codes code;
		OutputDebugString(api::to_wide_string(output, &code).c_str());
		if (code != core::codes::success) {
			std::wstring w_output = std::format(
				L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
				__FILEW__,
				__LINE__
			);
			OutputDebugString(w_output.c_str());
		}
	}

#else
	OutputDebugString(output.c_str());
#endif
}

void api::output_message(const std::string& message)
{
#if WIDE
	{
		core::codes code;
		OutputDebugString(api::to_wide_string(message, &code).c_str());
		if (code != core::codes::success) {
			std::wstring w_output = std::format(
				L"Problem with api::to_wide_string()\n File: {}\n Line: {}\n",
				__FILEW__,
				__LINE__
			);
			OutputDebugString(w_output.c_str());
		}
	}

#else
	OutputDebugString(message.c_str());
#endif
}




