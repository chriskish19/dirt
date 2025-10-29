/**********************************************************/
//
// File: main.cpp
//
// Purpose: main entry point
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_CMDLINE_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_SIM_INCLUDE_PATH
#include CORE_SIM_API_INCLUDE_PATH


#if !TEST_ENTRY // if TEST_ENTRY not equal to 1. See the header "CORE_DEFINES_INCLUDE_PATH".
int main(int argc, char* argv[]) {
    // core::t_out::enable_vt_mode();
    // global::terminal = std::make_unique<core::t_out>();
	
	/*
		going foward i will be implementing a win32 gui for dirt. 
		no more terminal.
	*/
	
	
	/*
		main object for parsing the command line or dirt list file
		into a vector of arg entrys. See the header "CORE_ARGS_INCLUDE_PATH" for definition of args enum.
		See the header "CORE_CMDLINE_INCLUDE_PATH" for definition of cmdline object.
	*/
    std::unique_ptr<core::cmdline> p_args = std::make_unique<core::cmdline>(argc,argv);
    {	// extra scope for the returned code object (using the same name "code" throughout main.cpp)
        core::codes code = p_args->parse();				// parses the command line or a dirt list file
        if (code != core::codes::success) {				// if there is an error exit the program and return the error code
            auto error_code = core::match_code(code);	// returns a code_pkg with a string message. See "CORE_CODES_INCLUDE_PATH".
            core::output_em(error_code);				// send a message out (could be terminal or gui)
            return static_cast<int>(code);				// return error code
        }
    }
	
	
    {
        core::codes code = p_args->validate();			// check if supplied arguments contradict each other
        if (code != core::codes::success) {				// same as above
            auto error_code = core::match_code(code);	// *************
            core::output_em(error_code);				// *************
            return static_cast<int>(code);				// *************
        }
    }


	/*
		main object for monitoring, processing, copying of files.
		See header "CORE_PROCESSOR_INCLUDE_PATH" for definition.
	*/
    std::unique_ptr<core::process> pfile_processor = std::make_unique<core::process>(p_args->m_entrys_v);
    {
        
		/* 
			copies files from found in copy entries and sets up necessary objects for watching directories
		*/
		core::codes code = pfile_processor->process_entry();	
        if (code != core::codes::success) {						// *************
            auto error_code = core::match_code(code);			// *************
            core::output_em(error_code);						// *************
            return static_cast<int>(code);						// *************
        }
    }
    
    {
		/* 
			Main directory watching loop, monitors directories for changes and reports the changes to 
			another loop which processes all the entries.
		*/
        core::codes code = pfile_processor->watch();			
        if (code != core::codes::success) {						// *************
            auto error_code = core::match_code(code);			// *************
            core::output_em(error_code);						// *************
            return static_cast<int>(code);						// *************
        }
    }
	
	// if we reach this point with no errors its a successful run
	// See the header "CORE_ARGS_INCLUDE_PATH" for actual code int value.
    return static_cast<int>(core::codes::success);
}


#else 
// TEST_ENTRY is equal to 1. See the header "CORE_DEFINES_INCLUDE_PATH". 

int main(int argc, char* argv[]) {
    // core::t_out::enable_vt_mode();
    // global::terminal = std::make_unique<core::t_out>();

    std::unique_ptr<core::cmdline> p_args = std::make_unique<core::cmdline>(argc, argv);
    {
        core::codes code = p_args->parse();
        if (code != core::codes::success) {
            auto error_code = core::match_code(code);
            core::output_em(error_code);
            return static_cast<int>(code);
        }
    }

    {
        core::codes code = p_args->validate();
        if (code != core::codes::success) {
            auto error_code = core::match_code(code);
            core::output_em(error_code);
            return static_cast<int>(code);
        }
    }

    std::unique_ptr<core::process> pfile_processor = std::make_unique<core::process>(p_args->m_entrys_v);
    {
        core::codes code = pfile_processor->process_entry();
        if (code != core::codes::success) {
            auto error_code = core::match_code(code);
            core::output_em(error_code);
            return static_cast<int>(code);
        }
    }

    std::unique_ptr<core::entry_sim> p_sim = std::make_unique<core::entry_sim>(TEST_FOLDER);

    auto code = p_sim->work(10);

    return static_cast<int>(code);

}

#endif