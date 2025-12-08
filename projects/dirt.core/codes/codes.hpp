/**********************************************************/
//
// File: codes.hpp
//
// Purpose: returnable error codes
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_STL_INCLUDE_PATH

namespace core {
	/* 
		common error codes in dirt.core
	*/
    enum class codes {
        success = 0,								// succesful completion
        uninit = 1,									// default initialization value
        invalid_file_path,							// file path not on system
        syntax_mistake,								// syntax mistake ( used when parsing dirt file )
        file_open_fail,								// cant open the given file path
        unknown,									// placeholder value
        invalid_cmd_args,							// one or more command line args are invalid
        invalid_io_handle,							// an error code returned from win32 when creating a completion handle
        handle_nullptr,								// an error code returned from win32 handles
        read_dir_changes_fail,						// an error code returned from win32 ReadDirectoryChangesW()
        no_valid_entries,							// no valid entries either from command line or dirt list file
        std_filesystem_exception_caught,			// standard exception caught in a try catch block
        unknown_exception_caught,					// unknown exception caught in a try catch block
        invalid_directory_path,						// an error code thats returned if a directory path is invalid
        exception_thrown_and_handled,				// indicates an exception was thrown and caught
        pointer_is_null,							// indicates a pointer is equal to nullptr ( no memory )
        to_wide_string_failed,                      // converting from narrow string to wide fail
        to_narrow_string_failed,                    // converting from wide to narrow string failed

		// win32 codes
		failed_to_register_class,					// failed to register win32 class on the system
		hwnd_fail,									// failed to create a window handle
		show_window_fail,							// failed to show window using ShowWindow() function
        getclientrect_fail,                         // failed to get the client rectangle for determining window heights and widths
    };
	
	
	
	/* 
		simple class to match an enum codes to a string message explaining the code in more detail
		also is throwable error object
	*/
    class code_pkg : public std::exception{
    public:
        code_pkg(codes code, std::string s_code)
            :m_code(code),m_s_code(s_code){ }

        const codes m_code;
        const std::string m_s_code;
		
		virtual const char* what(){
			return m_s_code.c_str();
		}
    };
	
	
	
	/* 
		instantiated objects for matching enum codes to string explainations 
	*/
    inline const code_pkg success_pkg(codes::success, "core::codes::success | message: successful execution.");
    inline const code_pkg c_uninit_pkg(codes::uninit, "core::codes::uninit | message: codes is uninitialized or default constructed.");
    inline const code_pkg invalid_file_path_pkg(codes::invalid_file_path, "core::codes::invalid_file_path | message: file path given does not exist on the system.");
    inline const code_pkg syntax_mistake_pkg(codes::syntax_mistake, "core::codes::syntax_mistake | message: syntax error in the given code.");
    inline const code_pkg file_open_fail_pkg(codes::file_open_fail, "core::codes::file_open_fail | message: error occurred while attempting to open a file.");
    inline const code_pkg c_unknown_pkg(codes::unknown, "core::codes::unknown | message: unknown error code.");
    inline const code_pkg invalid_cmd_args_pkg(codes::invalid_cmd_args, "core::codes::invalid_cmd_args | message: one or more command line arguments is invalid.");
    inline const code_pkg invalid_io_handle_pkg(codes::invalid_io_handle, "core::codes::invalid_io_handle | message: the handle was not created call the win32 function GetLastError() for more info.");
    inline const code_pkg handle_nullptr_pkg(codes::handle_nullptr, "core::codes::handle_nullptr | message: the windows win32 handle is nullptr,call the win32 function GetLastError() for more info.");
    inline const code_pkg read_dir_changes_fail_pkg(codes::read_dir_changes_fail, "core::codes::read_dir_changes_fail | message: the windows function ReadDirectoryChangesW() failed ,call the win32 function GetLastError() for more info.");
    inline const code_pkg no_valid_entries_pkg(codes::no_valid_entries, "core::codes::no_valid_entries | message: in the dirt_list.txt file given no valid entries were found, probably a syntax error.");
    inline const code_pkg std_filesystem_exception_caught_pkg(codes::std_filesystem_exception_caught, "core::codes::std_filesystem_exception_caught | message: standard file system exception caught and handled.");
    inline const code_pkg unknown_exception_caught_pkg(codes::unknown_exception_caught, "core::codes::unknown_exception_caught | message: unknown exception caught and handled.");
    inline const code_pkg invalid_directory_path_pkg(codes::invalid_directory_path, "core::codes::invalid_directory_path | message: directory path given either is not a directory or it doesnt exist on the system.");
    inline const code_pkg exception_thrown_and_handled_pkg(codes::exception_thrown_and_handled, "core::codes::exception_thrown_and_handled | message: an exception was thrown and handled in a try catch block.");
    inline const code_pkg pointer_is_null_pkg(codes::pointer_is_null, "core::codes::pointer_is_null | message: the pointer has no memory and is equal to nullptr");
    inline const code_pkg to_wide_string_failed_pkg(codes::to_wide_string_failed, "core::codes::to_wide_string_failed | message: converting from narrow string to wide failed");
    inline const code_pkg to_narrow_string_failed_pkg(codes::to_narrow_string_failed, "core::codes::to_narrow_string_failed | message: converting from wide string to narrow has failed");



	// win32 code pkgs
	inline const code_pkg failed_to_register_class_pkg(codes::failed_to_register_class,"core::codes::failed_to_register_class | message: the windows class has failed to be registerd on the system");
	inline const code_pkg hwnd_fail_pkg(codes::hwnd_fail, "core::codes::hwnd_fail | message: failed to create window handle, call win32 function GetLastError() for more info");
	inline const code_pkg show_window_fail_pkg(codes::show_window_fail, "core::codes::show_window_fail | message: failed to show window using the ShowWindow() function call win32 function GetLastError() for more info");
    inline const code_pkg getclientrect_fail_pkg(codes::getclientrect_fail, "core::codes::getclientrect_fail | message: failed to get client rect window dimensions, call win32 function GetLastError() for more info");
}