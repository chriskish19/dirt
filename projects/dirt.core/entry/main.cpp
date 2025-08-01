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
#include CORE_LOGGER_INCLUDE_PATH

#undef DrawText
using namespace ftxui;

int main(int argc, char* argv[]) {

    // global logging
    core::glb_sl = std::make_unique<core::system_log>();
    core::glb_el = std::make_unique<core::system_log>();

    std::vector<std::string> info_logs,
        error_logs;

    std::jthread ui_thread([&] {
        auto screen = ScreenInteractive::TerminalOutput();

        int info_selected = 0; // index of selected item
        int error_selected = 0;
        auto info_list = Menu(&info_logs, &info_selected);
        auto error_list = Menu(&error_logs, &error_selected);

        auto renderer = Renderer(Container::Vertical({ info_list, error_list }), [&] {
            core::glb_sl->latest(info_logs);
            core::glb_el->latest(error_logs);

            std::string details;
            if (info_list->Focused()) {
                if (!info_logs.empty() && info_selected >= 0 && info_selected < info_logs.size())
                    details = info_logs[info_selected];
            }
            else if (error_list->Focused()) {
                if (!error_logs.empty() && error_selected >= 0 && error_selected < error_logs.size())
                    details = error_logs[error_selected];
            }
            
            return vbox({
                text("DIRT (Directory Tool)") | center | bold,
                text("Select a log") | bold,
                window(
                    text("Info Logs"),
                    info_list->Render() | frame | size(HEIGHT, LESS_THAN, 10)
                ),
                window(
                    text("Error Logs"),
                    error_list->Render() | frame | size(HEIGHT, LESS_THAN, 10)
                ),
                separator(),
                window(
                    text("Log Details"),
                    paragraph(details) | frame | size(HEIGHT, LESS_THAN, 8)
                ),
                }); // vbox
            }); // renderer
        screen.Loop(renderer);
        }); // thread



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