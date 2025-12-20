#include "main_entry.hpp"
/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_MAIN_ENTRY_INCLUDE_PATH



core::backend::backend(const std::vector<arg_entry>& v)
    :process(v)
{
	
}

core::codes core::backend::run()
{
    {

        /*
            copies files from found in copy entries and sets up necessary objects for watching directories
        */
        core::codes code = process_entry();
        if (code != core::codes::success) {						// if there is an error exit the program and return the error code
            auto error_code = core::match_code(code);			// returns a code_pkg with a string message. See "CORE_CODES_INCLUDE_PATH".
            core::output_em(error_code);						// send a message out (could be terminal or gui)
            return code;						                // return error code
        }
    }

    {
        /*
            Main directory watching loop, monitors directories for changes and reports the changes to
            another loop which processes all the entries.
        */
        core::codes code = watch();
        if (code != core::codes::success) {						// if there is an error exit the program and return the error code
            auto error_code = core::match_code(code);			// returns a code_pkg with a string message. See "CORE_CODES_INCLUDE_PATH".
            core::output_em(error_code);						// send a message out (could be terminal or gui)
            return code;						                // return error code
        }
    }

    // if we reach this point with no errors its a successful run
    // See the header "CORE_ARGS_INCLUDE_PATH" for actual code int value.
    return core::codes::success;
}
