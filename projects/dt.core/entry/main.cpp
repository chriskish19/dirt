/**********************************************************/
//
// File: main.cpp
//
// Purpose: main entry point
//
// Project: dt.core
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
    LoadLibrary(L"Msftedit.dll");
    core::codes code;
    auto entry_v = api::windows_cmd_line(&code);
    if (code != core::codes::success) {
        auto pkg = api::match_code(code);
        api::output_cp(pkg);
    }
    try {
        std::unique_ptr<core::main::gui_entry> p_gui = std::make_unique<core::main::gui_entry>(entry_v);
        p_gui->go();
    }
    catch (const core::code_pkg& cpe) {
        api::output_cp(cpe);
        return static_cast<int>(core::codes::exception_thrown_and_handled);
    }
    catch (...) {
        api::output_cp(core::unknown_exception_caught_pkg);
        return static_cast<int>(core::codes::unknown_exception_caught);
    }
    return static_cast<int>(core::codes::success);
}
#endif
#if WIN32_GUI_AND_TERMINAL
std::vector<core::arg_entry> entry_v;
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    try {
        std::unique_ptr<core::gui_with_terminal_entry> p_gui = std::make_unique<core::gui_with_terminal_entry>(entry_v);
        p_gui->go();
    }
    catch (const core::code_pkg& cpe) {
        MessageBox(NULL, api::to_wide_string(cpe.m_s_code).c_str(), L"EXCEPTION", MB_OK | MB_ICONERROR);
        return static_cast<int>(core::codes::exception_thrown_and_handled);
    }
    catch (...) {
        MessageBox(NULL, api::to_wide_string(core::unknown_exception_caught_pkg.m_s_code).c_str(), L"EXCEPTION", MB_OK | MB_ICONERROR);
        return static_cast<int>(core::codes::unknown_exception_caught);
    }
   
    return static_cast<int>(core::codes::success);
}
int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    PWSTR pCmdLine = GetCommandLineW();
    {
        core::codes code;
        entry_v = api::cmd_line(argc, argv, &code);
        if (code != core::codes::success) {
            std::cout << api::match_code(code).m_s_code << '\n';
            std::cout << api::get_location() << '\n';
        }
    }
    {
        core::codes code = api::validate(entry_v);
        if (code != core::codes::success) {
            std::cout << api::match_code(code).m_s_code << '\n';
            std::cout << api::get_location() << '\n';
        }
    }
    SetConsoleOutputCP(CP_UTF8);
    return wWinMain(hInstance, nullptr, pCmdLine, SW_SHOW);
}
#endif