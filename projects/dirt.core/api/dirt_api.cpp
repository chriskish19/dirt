#include "dirt_api.hpp"
/**********************************************************/
//
// File: dirt_api.cpp
//
// Purpose: dirt_api.hpp definitions
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_API_INCLUDE_PATH

core::arg_pkg core::match_arg_enum(args arg)
{
	switch (arg) {
	case args::copy:
		return copy_pkg;
	case args::dirt_list_path:
		return dirt_list_path_pkg;
	case args::mirror:
		return mirror_pkg;
	case args::new_files_only:
		return new_files_only_pkg;
	case args::no_deletes:
		return no_deletes_pkg;
	case args::uninit:
		return uninit_pkg;
	case args::watch:
		return watch_pkg;
	case args::src:
		return src_pkg;
	case args::dst:
		return dst_pkg;
	case args::recursive:
		return recursive_pkg;
	default:
		return unknown_pkg;
	}
}

core::arg_pkg core::match_s_arg(const std::string& arg)
{
	if (arg == copy_pkg.m_s_arg) {
		return copy_pkg;
	}
	if (arg == dirt_list_path_pkg.m_s_arg) {
		return dirt_list_path_pkg;
	}
	if (arg == mirror_pkg.m_s_arg) {
		return mirror_pkg;
	}
	if (arg == new_files_only_pkg.m_s_arg) {
		return new_files_only_pkg;
	}
	if (arg == no_deletes_pkg.m_s_arg) {
		return no_deletes_pkg;
	}
	if (arg == uninit_pkg.m_s_arg) {
		return uninit_pkg;
	}
	if (arg == watch_pkg.m_s_arg) {
		return watch_pkg;
	}
	if (arg == src_pkg.m_s_arg) {
		return src_pkg;
	}
	if (arg == dst_pkg.m_s_arg) {
		return dst_pkg;
	}
	if (arg == recursive_pkg.m_s_arg) {
		return recursive_pkg;
	}
	return unknown_pkg;
}

core::code_pkg core::match_code(codes code)
{
	switch (code) {
	case codes::success:
		return success_pkg;
	case codes::file_open_fail:
		return file_open_fail_pkg;
	case codes::invalid_file_path:
		return invalid_file_path_pkg;
	case codes::syntax_mistake:
		return syntax_mistake_pkg;
	case codes::uninit:
		return c_uninit_pkg;
	case codes::invalid_cmd_args:
		return invalid_cmd_args_pkg;
	case codes::invalid_io_handle:
		return invalid_io_handle_pkg;
	case codes::no_valid_entries:
		return no_valid_entries_pkg;
	case codes::read_dir_changes_fail:
		return read_dir_changes_fail_pkg;
	case codes::std_filesystem_exception_caught:
		return std_filesystem_exception_caught_pkg;
	case codes::invalid_directory_path:
		return invalid_directory_path_pkg;
	case codes::unknown_exception_caught:
		return unknown_exception_caught_pkg;
	case codes::exception_thrown_and_handled:
		return exception_thrown_and_handled_pkg;
	case codes::pointer_is_null:
		return pointer_is_null_pkg;
	case codes::failed_to_register_class:
		return failed_to_register_class_pkg;
	case codes::hwnd_fail:
		return hwnd_fail_pkg;
	case codes::show_window_fail:
		return show_window_fail_pkg;
	default:
		return c_unknown_pkg;
	}
}

std::string core::get_location(std::source_location sl)
{
	std::string function_name = sl.function_name();
	std::string line = std::to_string(sl.line());
	std::string column = std::to_string(sl.column());
	std::string file_name = sl.file_name();

	std::string temp = std::format("File name: {} \n - column: {} \n - line: {} \n - function name: {} \n", file_name, column, line, function_name);
	return temp;
}

void core::output_em(const code_pkg cp, const std::string location)
{
	std::string message = cp.m_s_code + '\n' 
		+ location + '\n';

	main_output(message);
}

std::vector<core::args> core::validate_args(const std::vector<arg_pkg>& args)
{
	std::vector<core::args> arg_v;
	for (auto& arg : args) {
		arg_v.push_back(arg.m_arg);
	}

	group_2 g2;
	group_3 g3;
	group_4 g4;
	group_5 g5;

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

std::vector<core::arg_entry> core::parse_file(const std::filesystem::path& p, core::codes* code_p)
{
	// file object stream
	std::ifstream file;
	
	// try to open the file from the path p
	file.open(p);
	
	// if it fails to open we return an empty vector and set 
	// the proper value for the error code
	if (file.is_open() == false) {
		*code_p = codes::file_open_fail;
		return {};
	}
	
	// see CORE_ARGS_INCLUDE_PATH for definition.
	// each listed entry in the file needs an object 
	// placeholder then it gets added to the vector.
	arg_entry entry;	
	
	// the return vector
	std::vector<arg_entry> entry_v;

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
			if (found != gbl_args_mp.end()) {
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
					if (found != gbl_args_mp.end()) {				// if found add them to the vector
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
			entry = arg_entry(); // reset
		}
	}
	
	// we successfully read the file and got the entries
	*code_p = codes::success;
	return entry_v;
}

std::vector<core::args> core::arg_pkg_to_args(const std::vector<arg_pkg>& args_pkg_v)
{
	std::vector<core::args> args_v;
	for (auto& arg : args_pkg_v) {
		args_v.push_back(arg.m_arg);
	}
	return args_v;
}

bool core::validate_entry(const arg_entry& e)
{
	
	group_2 g2;
	group_3 g3;
	group_4 g4;
	group_5 g5;

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

std::string core::get_last_error_w32()
{
	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0) {
		return {}; // No error message has been recorded 
	}

	LPSTR messageBuffer = nullptr;

	// Format the error message 
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&messageBuffer,
		0,
		NULL
	);

	// Copy the error message into a wide string
	std::string message(messageBuffer, size);

	// Free the buffer allocated by FormatMessage 
	LocalFree(messageBuffer);

	return message;
}

void core::output_entry(const arg_entry& e)
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
	
	// output to console or some other display to the user
	main_output(message);
}

std::string core::text_output_fse(const std::filesystem::filesystem_error& e)
{
	// create the string error message
	std::string message = "Message: " + std::string(e.what()) + '\n' 
		+ "Path 1: " + e.path1().string() + '\n'
		+ "Path 2: " + e.path2().string() + '\n';
	
	return message;
}

std::uintmax_t core::file_numbers(const std::filesystem::path& p)
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
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}
	
	// exception thrown, error occured so return 0
	return 0;
}

std::unordered_set<core::directory_info> core::get_all_directories(const std::filesystem::path& p)
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
				directory_info di;
				di.p = entry.path();
				di.number_of_files = file_numbers(entry.path());
				di_set.emplace(di);
			}
		}
		
		// the set is filled up so return it
		return di_set;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}
	
	// an exception was thrown, lets return a blank set
	return {};
}

core::codes core::copy_directory_only(const std::filesystem::path& dst, const std::filesystem::path& src)
{
	try {
		// checks if src and dst are directories
		if (std::filesystem::is_directory(dst) == false or std::filesystem::is_directory(src) == false) {
			// if they are not directories return an error code
			return codes::invalid_directory_path;
		}
		
		// copy src to dst
		std::filesystem::copy(src, dst, std::filesystem::copy_options::update_existing);
		
		// success
		return codes::success;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}
	
	// an exception was thrown, return the code
	return codes::exception_thrown_and_handled;
}

core::codes core::copy_directory_recursive(const std::filesystem::path& dst, const std::filesystem::path& src)
{
	try {
		if (std::filesystem::is_directory(dst) == false or std::filesystem::is_directory(src) == false) {
			return codes::invalid_directory_path;
		}

		std::filesystem::copy(src, dst, 
			std::filesystem::copy_options::update_existing |
			std::filesystem::copy_options::recursive);

		return codes::success;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}
	return codes::exception_thrown_and_handled;
}

std::string core::action_to_string(file_action action)
{
	switch (action) {
	case file_action::copy:
		return "copy the file from the source directory to the destination directory";
	case file_action::delete_dst:
		return "delete the file in the destination directory";
	case file_action::modified:
		return "file has been modified";
	case file_action::new_name:
		return "file has been renamed, this is the new name";
	case file_action::previous_name:
		return "file has been renamed, this is the old name";
	case file_action::uninit:
		return "the file action enum is uninitialized, no value has been set";
	default:
		return "no matching file action enum found";
	}
}

std::string core::file_type_to_string(std::filesystem::file_type type) {
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

void core::output_filesystem_ec(std::error_code ec)
{
	std::string message = "filesystem error (" + std::to_string(ec.value()) + "): " + ec.message() + '\n';
	main_output(message);
}

std::vector<core::arg_entry> core::get_specific_entries(const std::vector<arg_entry>& v, args specific_arg)
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

std::uintmax_t core::total_size(const std::filesystem::path& p)
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
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}
	return 0;
}

std::vector<std::queue<core::file_entry>> core::split_queue(std::queue<file_entry>& buffer_q, std::size_t number_of_qs)
{
	std::vector<std::queue<core::file_entry>> file_entry_v_q;
	auto buffer_q_size = buffer_q.size();

	if (buffer_q_size == 0) {
		return {};
	}

	auto q_size = buffer_q_size / number_of_qs;

	while (buffer_q.empty() == false) {
		std::queue<file_entry> q;
		while (q.size() < q_size and buffer_q.empty() == false) {
			file_entry entry = buffer_q.front();
			q.emplace(entry);
			buffer_q.pop();
		}
		file_entry_v_q.push_back(q);
	}
	return file_entry_v_q;
}


std::string core::output_entry_data(const file_entry& entry, const std::string& name)
{
	std::string message = '\n' +
		name + '\n'
		+ "Source Path: " + entry.src_p.string() + '\n'
		+ "Destination Path: " + entry.dst_p.string() + '\n'
		+ "Action: " + action_to_string(entry.action) + '\n'
		+ "File type: " + file_type_to_string(entry.src_s.type()) + '\n';
	
	return message;
	 
}



UINT core::get_window_width(HWND window_handle)
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        throw core::getclientrect_fail_pkg;
    }

    return rc.right - rc.left;
}

UINT core::get_window_height(HWND window_handle) {
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        throw core::getclientrect_fail_pkg;
    }

    return rc.bottom - rc.top;
}

UINT core::get_window_width(HWND window_handle,core::codes* code) noexcept
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        *code = core::codes::getclientrect_fail;
        return 0;
    }

    return rc.right - rc.left;
}

UINT core::get_window_height(HWND window_handle, core::codes* code) noexcept
{
    RECT rc = {};
    if (GetClientRect(window_handle, &rc) == FALSE) {
        *code = core::codes::getclientrect_fail;
        return 0;
    }

    return rc.bottom - rc.top;
}

std::wstring core::to_wide_string(const char* narrow) {
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
        throw core::to_wide_string_failed_pkg;

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

std::wstring core::to_wide_string(const std::string& narrow) {
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
        throw core::to_wide_string_failed_pkg;

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

std::string core::to_narrow_string(const wchar_t* wide) {
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
		throw core::to_narrow_string_failed_pkg;

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

std::string core::to_narrow_string(const std::wstring& wide) {
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
		throw core::to_narrow_string_failed_pkg;

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

std::wstring core::to_wide_string(const char* narrow, core::codes* code_p) {
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
    boolean heap_alloc = false;

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

std::wstring core::to_wide_string(const std::string& narrow, core::codes* code_p) {
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

std::string core::to_narrow_string(const wchar_t* wide, core::codes* code_p) {
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

std::string core::to_narrow_string(const std::wstring& wide, core::codes* code_p) {
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

std::vector<core::arg_entry> core::cmd_line(int argc, char* argv[], codes* code)
{	
	std::vector<core::arg_entry> entry_v;

	bool next_is_dtlist = false;
	bool using_dtlist = false;

	fs::path dtlist_path;

	args last_arg = args();

	arg_entry single_entry;

	for (int i = 0; i < argc; ++i) {
		std::string word = argv[i];
		
		if (last_arg == args::src) {
			single_entry.src_p = word;
		}
		else if (last_arg == args::dst) {
			single_entry.dst_p = word;
		}

		if (next_is_dtlist == true) {
			dtlist_path = word;
			using_dtlist = true;
			break;
		}

		auto found = gbl_args_mp.find(word);
		if (found != gbl_args_mp.end()) {
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
		entry_v = parse_file(dtlist_path,code);
		return entry_v;
	}
	else { // using one entry cmdline
		entry_v.push_back(single_entry);
		return entry_v;
	}
	return {};
}

core::codes core::validate(std::vector<core::arg_entry>& v)
{
	std::vector<std::size_t> invalid_entrys_v;
	for (const auto& entry : v) {
		if (validate_entry(entry) == false) {
			main_output("error : invalid entry\n");
			output_entry(entry);
			invalid_entrys_v.push_back(entry.entry_number);
		}
	}

	for (auto i : invalid_entrys_v) {
		v.erase(v.begin() + (i - 1));
	}

	if (v.empty()) {
		return codes::no_valid_entries;
	}
	return codes::success;
}

std::string core::time_now(const std::string& message)
{
	try {
		auto now = std::chrono::system_clock::now();
		std::string time = std::format("[{}]", now);
		return time + message;
	}
	catch (const std::exception& e) {
		std::cout << "exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "unknown exception caught..." << std::endl;
	}
	// exception thrown we return nothing
	return {};
}





