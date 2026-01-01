
/**********************************************************/
//
// File: dt_window.cpp
//
// Purpose: dt_window.hpp definitions
//
// Project: dt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_WINDOW_INCLUDE_PATH

core::window::starter::starter(const string& class_name)
{
    m_c_name = class_name;
}

LRESULT core::window::starter::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // reroute to private window proc
    starter* p_window_rerouter = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        // Store the pointer to the window instance in the user data associated with the HWND.
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        p_window_rerouter = (starter*)pCreate->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)p_window_rerouter);
    }
    else
    {
        // Retrieve the pointer to the window instance from the user data associated with the HWND.
        p_window_rerouter = (starter*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (p_window_rerouter)
    {
        return p_window_rerouter->ThisWindowProc(hwnd, uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

LRESULT core::window::starter::ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY:
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        break;
    } // end of switch (uMsg)

    // no default switches needed
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

core::codes core::window::starter::window_settings()
{
    m_wc.lpfnWndProc = WindowProc;
    m_wc.hInstance = m_hinst;
    m_wc.lpszClassName = m_c_name.c_str();

    if (RegisterClass(&m_wc) == FALSE) {
        return core::codes::failed_to_register_class;
    }
    return core::codes::success;
}

core::codes core::window::starter::create_window()
{
    m_window_handle = CreateWindowEx(
        0,                                              // Optional window styles.
        m_c_name.c_str(),                               // Window class
        m_title.c_str(),                                // Window text
        WS_OVERLAPPEDWINDOW,                            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,                                           // Parent window    
        NULL,                                           // Load the menu here
        m_hinst,                                        // Instance handle
        this                                            // Additional application data
    );

    if (m_window_handle == nullptr) {
        return core::codes::hwnd_fail;
    }

    
    if (ShowWindow(m_window_handle, SW_SHOW) > 0) {
        return core::codes::show_window_fail;
    }
    
    return core::codes::success;
}

core::codes core::window::starter::message_pump()
{
    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return core::codes::success;
}

core::window::window::window()
{
    m_title = ROS("Main Window - CORE DT");
    m_c_name = ROS("MAIN WINDOW");

    {
        core::codes code;
        code = window_settings();
        if (code != core::codes::success) {
            // throws a code_pkg object exception
            throw api::match_code(code);
        }
    }

    {
        core::codes code;
        code = create_window();
        if (code != core::codes::success) {
            throw api::match_code(code);
        }
    }



    {
        core::codes code;
        code = add_dynamic_menu(m_window_handle);
        if (code != codes::success) {
            throw api::match_code(code);
        }
    }


    {
        core::codes code;
        code = set_icon(m_window_handle, DT_ICON_PATH);
        if (code != codes::success) {
            throw api::match_code(code);
        }
    }
}

LRESULT core::window::window::ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_CREATE:
        {
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(icex);
            icex.dwICC = ICC_PROGRESS_CLASS;
            InitCommonControlsEx(&icex);
            break;
        }
    
        case WM_SIZING:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // lParam holds a pointer to a RECT structure
            RECT* lw_rect_p = reinterpret_cast<RECT*>(lParam);
            FillRect(hdc, lw_rect_p, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
            break;
        }
    } // end of switch (uMsg)

    // no default switch needed
    return starter::ThisWindowProc(hwnd, uMsg, wParam, lParam);
}

core::codes core::window::window::add_dynamic_menu(HWND window_handle)
{
    HMENU hMenu = CreateMenu();
    if (hMenu == nullptr) {
        return codes::menu_fail;
    }

    HMENU hFileMenu = CreatePopupMenu();
    if (hFileMenu == nullptr) {
        return codes::menu_fail;
    }

    HMENU hHelpMenu = CreatePopupMenu();
    if (hHelpMenu == nullptr) {
        return codes::menu_fail;
    }

    HMENU hViewMenu = CreatePopupMenu();
    if (hViewMenu == nullptr) {
        return codes::menu_fail;
    }

    if (!AppendMenu(hFileMenu, MF_STRING, static_cast<UINT_PTR>(core::gui::window_ids::open), L"&Open")) {
        return codes::menu_fail;
    }
    if (!AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr)) {
        return codes::menu_fail;
    }
    if (!AppendMenu(hFileMenu, MF_STRING, static_cast<UINT_PTR>(core::gui::window_ids::exit), L"E&xit")) {
        return codes::menu_fail;
    }

    if (!AppendMenu(hHelpMenu, MF_STRING, static_cast<UINT_PTR>(core::gui::window_ids::help), L"&About")) {
        return codes::menu_fail;
    }

    if (!AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File")) {
        return codes::menu_fail;
    }
    if (!AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help")) {
        return codes::menu_fail;
    }
    if (!AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, L"&View")) {
        return codes::menu_fail;
    }


    if (!AppendMenu(hViewMenu, MF_STRING, static_cast<UINT_PTR>(core::gui::window_ids::show_system_logger), L"&System Logger")) {
        return codes::menu_fail;
    }

    if (!SetMenu(window_handle, hMenu)) {
        return codes::menu_fail;
    }

    return codes::success;
}

core::codes core::window::window::set_icon(HWND hwnd, const fs::path& icon_path) {
    HICON hIcon = (HICON)LoadImage(NULL, icon_path.c_str(), IMAGE_ICON, 0 ,0 , LR_LOADFROMFILE | LR_DEFAULTSIZE);
    
    // Set taskbar icon
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    
    // Set window title bar icon
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    
    return codes::success;
}

LRESULT core::window::dt_window::ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);  // Extract the menu item ID
        switch (wmId) {
        case static_cast<int>(core::gui::window_ids::help):
        {
            MessageBox(nullptr,
                APP_INFO,
                L"Directory Tool",
                MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);
            break;
        }
        
        case static_cast<int>(core::gui::window_ids::console):
        {
            break;
        }

        case static_cast<int>(core::gui::window_ids::b_front):
        {
            m_p_main_ui->m_launch_b.action(core::gui::button_state::pressed);
            break;
        }

        case static_cast<int>(core::gui::window_ids::b_refresh):
        {
            m_p_main_ui->m_refresh_b.action(core::gui::button_state::pressed);
            break;
        }

        case static_cast<int>(core::gui::window_ids::lb_box):
        {
            m_p_main_ui->m_front_lb.action(static_cast<core::gui::listbox_commands>(HIWORD(wParam)));
            break;
        }

        case static_cast<int>(core::gui::window_ids::label):
        {
            m_p_main_ui->m_lb_label.action(static_cast<core::gui::label_commands>(HIWORD(wParam)));
            break;
        }

        case static_cast<int>(core::gui::window_ids::show_system_logger):
        {
            if (m_show_logger == true) {
                ShowWindow(core::logger::glb_sl->get_window_handle(), SW_HIDE);
                m_show_logger = false;
            }
            else {
                ShowWindow(core::logger::glb_sl->get_window_handle(), SW_SHOW);
                m_show_logger = true;
            }

            break;
        }

        case static_cast<int>(core::gui::window_ids::b_copy):
        {
            m_p_main_ui->m_copy_b.action(core::gui::button_state::pressed);
            break;
        }

        case static_cast<int>(core::gui::window_ids::b_new):
        {
            m_p_main_ui->m_new_b.action(core::gui::button_state::pressed);
            break;
        }

        case static_cast<int>(core::gui::window_ids::b_open):
        {
            m_p_main_ui->m_open_b.action(core::gui::button_state::pressed);
            break;
        }

        case static_cast<int>(core::gui::window_ids::b_save):
        {
            m_p_main_ui->m_save_b.action(core::gui::button_state::pressed);
            break;
        }

        default:
            break;
        } // end of switch (wmId)
        break;
    }

    } // end of switch (uMsg)

    // no default switch needed
    return window::ThisWindowProc(hwnd, uMsg, wParam, lParam);
}
