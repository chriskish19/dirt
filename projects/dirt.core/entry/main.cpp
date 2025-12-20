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
#include CORE_PROCESSOR_INCLUDE_PATH
#include CORE_LOGGER_INCLUDE_PATH
#include CORE_SIM_INCLUDE_PATH
#include CORE_SIM_API_INCLUDE_PATH

#if TERMINAL_BUILD
#if !TEST_ENTRY // if TEST_ENTRY not equal to 1. See the header "CORE_DEFINES_INCLUDE_PATH".
int main(int argc, char* argv[]) {
    std::unique_ptr<core::terminal_entry> p_terminal = std::make_unique<core::terminal_entry>(argc, argv);

    {
        core::codes code = p_terminal->init();
        if (code != core::codes::success) {
            auto error_code = core::match_code(code);
            std::cout << error_code.m_s_code << '\n';
            return static_cast<int>(code);
        }
    }

    p_terminal->go();


	// if we reach this point with no errors its a successful run
	// See the header "CORE_ARGS_INCLUDE_PATH" for actual code int value.
    return static_cast<int>(core::codes::success);
}


#else 
// TEST_ENTRY is equal to 1. See the header "CORE_DEFINES_INCLUDE_PATH". 

int main(int argc, char* argv[]) {
    

    

    return static_cast<int>(code);

}

#endif
#endif

#if WIN32_GUI_BUILD


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    


}

#endif