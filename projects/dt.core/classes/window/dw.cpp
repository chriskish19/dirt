
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#if! UNDER_CONSTRUCTION

/**********************************************************/
//
// File: dw.cpp (dirt window)
//
// Purpose: dw.hpp definitions
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_WINDOW_INCLUDE_PATH

core::starter::starter(const string& class_name)
{
    m_c_name = class_name;
}

LRESULT core::starter::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

LRESULT core::starter::ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    } // end of switch (uMsg)

    // no default switches needed
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

core::codes core::starter::window_settings()
{
    m_wc.lpfnWndProc = WindowProc;
    m_wc.hInstance = m_hinst;
    m_wc.lpszClassName = m_c_name.c_str();

    if (RegisterClass(&m_wc) == FALSE) {
        return core::codes::failed_to_register_class;
    }
    return core::codes::success;
}

core::codes core::starter::create_window()
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

core::codes core::starter::message_pump()
{
    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return core::codes::success;
}

core::window::window()
{
    m_title = ROS("Main Window - CORE DIRT");
    m_c_name = ROS("MAIN WINDOW");
}

void core::window::go()
{
    {
        core::codes code;
        code = window_settings();
        if (code != core::codes::success) {
            // throws a code_pkg object exception
			throw match_code(code);
        }
    }

    {
        core::codes code;
        code = create_window();
        if (code != core::codes::success) {
            throw match_code(code);
        }
    }

    m_main_ui = ui(m_window_handle, nullptr, m_hinst, nullptr);
    m_main_ui.m_front_lb.add_string(ROS("Test string"));
    
    {
        juju_codes code;
        code = add_dynamic_menu(m_window_handle);
        if (code != juju_codes::success) {
            code_description code_obj = match_code(code);
            throw jujubee_error(code_obj);
        }
    }

    
    {
        juju_codes code;
        code = set_icon(m_window_handle, JUJUBEE_ICON_PATH);
        if (code != juju_codes::success) {
            code_description code_obj = match_code(code);
            throw jujubee_error(code_obj);
        }
    }
    

    {
        juju_codes code;
        code = message_pump();
        if (code != juju_codes::success) {
            code_description code_obj = match_code(code);
            throw jujubee_error(code_obj);
        }
    }
}

LRESULT juju::window::ThisWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);  // Extract the menu item ID
            switch (wmId) {
                case static_cast<int>(window_ids::console):
                {
                    break;
                }

                case static_cast<int>(window_ids::b_front):
                {
                    m_main_ui.m_launch_b.action(button_state::pressed);
                    break;
                }

                case static_cast<int>(window_ids::b_refresh):
                {
                    m_main_ui.m_refresh_b.action(button_state::pressed);
                    break;
                }

                case static_cast<int>(window_ids::lb_box):
                {
                    m_main_ui.m_front_lb.action(static_cast<listbox_commands>(HIWORD(wParam)));
                    break;
                }

                case static_cast<int>(window_ids::label):
                {
                    m_main_ui.m_lb_label.action(static_cast<label_commands>(HIWORD(wParam)));
                    break;
                }

                default:
                    break;
            } // end of switch (wmId)
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

juju::juju_codes juju::window::add_dynamic_menu(HWND window_handle)
{
    HMENU hMenu = CreateMenu();
    if (hMenu == nullptr) {
        return juju_codes::menu_fail;
    }

    HMENU hFileMenu = CreatePopupMenu();
    if (hFileMenu == nullptr) {
        return juju_codes::menu_fail;
    }

    HMENU hHelpMenu = CreatePopupMenu();
    if (hHelpMenu == nullptr) {
        return juju_codes::menu_fail;
    }

    if (!AppendMenu(hFileMenu, MF_STRING, static_cast<UINT_PTR>(window_ids::open), L"&Open")) {
        return juju_codes::menu_fail;
    }
    if (!AppendMenu(hFileMenu, MF_SEPARATOR, 0, nullptr)) {
        return juju_codes::menu_fail;
    }
    if (!AppendMenu(hFileMenu, MF_STRING, static_cast<UINT_PTR>(window_ids::exit), L"E&xit")) {
        return juju_codes::menu_fail;
    }

    if (!AppendMenu(hHelpMenu, MF_STRING, static_cast<UINT_PTR>(window_ids::help), L"&About")) {
        return juju_codes::menu_fail;
    }

    if (!AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"&File")) {
        return juju_codes::menu_fail;
    }
    if (!AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, L"&Help")) {
        return juju_codes::menu_fail;
    }

    if (!SetMenu(window_handle, hMenu)) {
        return juju_codes::menu_fail;
    }

    return juju_codes::success;
}

juju::juju_codes juju::window::set_icon(HWND hwnd, const fs::path& icon_path) {
    HICON hIcon = (HICON)LoadImage(NULL, icon_path.c_str(), IMAGE_ICON, 0 ,0 , LR_LOADFROMFILE | LR_DEFAULTSIZE);
    
    // Set taskbar icon
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    
    // Set window title bar icon
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    
    return juju::juju_codes::success;
}

#endif