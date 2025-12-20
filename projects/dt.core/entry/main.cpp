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
#include CORE_MAIN_ENTRY_INCLUDE_PATH


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
    std::unique_ptr<core::test_terminal_entry> p_test_terminal = std::make_unique<core::test_terminal_entry>();
    p_test_terminal->go();
    return static_cast<int>(core::codes::success);
}

#endif
#endif

#if WIN32_GUI_BUILD


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    


}

#endif