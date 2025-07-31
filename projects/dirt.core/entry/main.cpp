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
#include CORE_FTXUI_INCLUDE_PATH
#include CORE_STL_INCLUDE_PATH
#include CORE_ARGS_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH
#include CORE_CMDLINE_INCLUDE_PATH
#include CORE_PROCESSOR_INCLUDE_PATH


int main(int argc, char* argv[]) {

    std::jthread ui_thread([] {
        float progress = 0.0f;

        auto screen = ftxui::ScreenInteractive::TerminalOutput();

        auto renderer = ftxui::Renderer([&] {
            return ftxui::vbox({
                // Simple header
                ftxui::text("DIRT API") | ftxui::bold | ftxui::center,

                // Progress bar
                ftxui::gauge(progress) | ftxui::border,

                // Placeholder content
                ftxui::text("Working...") | ftxui::center,
                }) | ftxui::border;
            });

        screen.Loop(renderer);
        });




    std::unique_ptr<core::cmdline> p_args = std::make_unique<core::cmdline>(argc,argv);
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
    
    {
        core::codes code = pfile_processor->watch();
        if (code != core::codes::success) {
            auto error_code = core::match_code(code);
            core::output_em(error_code);
            return static_cast<int>(code);
        }
    }

    return static_cast<int>(core::codes::success);
}