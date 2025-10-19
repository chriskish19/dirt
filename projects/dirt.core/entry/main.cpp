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
#include CORE_GLB_INCLUDE_PATH

#include <curses.h>

#if !TEST_ENTRY
int main(int argc, char* argv[]) {
    core::t_out::enable_vt_mode();
    global::terminal = std::make_unique<core::t_out>();


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
#else
// testing entry point

int main(int argc, char* argv[]) {
    core::t_out::enable_vt_mode();
    global::terminal = std::make_unique<core::t_out>();

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