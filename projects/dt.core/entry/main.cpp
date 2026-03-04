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
int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    try {
        std::unique_ptr<core::main::Cterminal> p_terminal = std::make_unique<core::main::Cterminal>(argc,argv);
        p_terminal->go();
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
#if WIN32_GUI_BUILD
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    LoadLibrary(L"Msftedit.dll");
    core::logger::glb_sl = std::make_unique<core::logger::system_log_window>();
    try {
        std::unique_ptr<core::main::Cmain> p_gui = std::make_unique<core::main::Cmain>(core::main::build::gui);
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    core::logger::glb_sl = std::make_unique<core::logger::system_log_window>();
    try {
        std::unique_ptr<core::main::Cmain> p_gui = std::make_unique<core::main::Cmain>(core::main::build::gui_with_terminal);
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
int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandleW(NULL);
    PWSTR pCmdLine = GetCommandLineW();
    SetConsoleOutputCP(CP_UTF8);
    return wWinMain(hInstance, nullptr, pCmdLine, SW_SHOW);
}
#endif