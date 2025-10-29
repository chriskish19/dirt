/***********************************************
*
* File: ui.cpp
*
* Purpose: ui.hpp definitions
*
* Project: jujubee
*
************************************************/

#include NAMES_INCLUDE
#include UI_INCLUDE_PATH

juju::ui::ui(HWND window, HMENU menu, HINSTANCE hinst, LPVOID lp)
{

    // front button setup
    m_launch_bd.window = window;
    m_launch_bd.menu = (HMENU)window_ids::b_front;
    m_launch_bd.hinst = hinst;
    m_launch_bd.lpParam = lp;
    m_launch_bd.button_caller = [this](button_state bs) {
        this->front_button_action(bs);
        };

    m_launch_b = button(m_launch_bd);
    m_launch_b.create();

    // refresh button setup
    m_refresh_bd.window = window;
    m_refresh_bd.menu = (HMENU)window_ids::b_refresh;
    m_refresh_bd.hinst = hinst;
    m_refresh_bd.lpParam = lp;
    m_refresh_bd.button_caller = [this](button_state bs) {
        this->refresh_button_action(bs);
        };

    m_refresh_b = button(m_refresh_bd);
    m_refresh_b.create();

    // front listbox setup
    m_front_lb_ld.window = window;
    m_front_lb_ld.menu = (HMENU)window_ids::lb_box;
    m_front_lb_ld.hinst = hinst;
    m_front_lb_ld.lpParam = lp;
    m_front_lb_ld.listbox_caller = [this](listbox_commands lc) {
        this->front_listbox_action(lc);
        };

    m_front_lb = listbox(m_front_lb_ld);
    m_front_lb.create();

	// front label setup
	m_lb_label_d.window = window;
	m_lb_label_d.menu = (HMENU)window_ids::label;
	m_lb_label_d.hinst = hinst;
	m_lb_label_d.lpParam = lp;
	m_lb_label_d.label_caller = [this](label_commands command) {
		this->lb_label_action(command);
		};

	m_lb_label = label(m_lb_label_d);
	m_lb_label.create();

}
void juju::ui::front_button_action(button_state bs)
{
	switch (bs) {
		case button_state::rest:
		{
			return;
		}

		case button_state::pressed:
		{
			MessageBox(nullptr, L"Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
			break;
		}

		case button_state::released:
		{
			break;
		}

		case button_state::holding:
		{
			break;
		}

	default:
		break;
	}
}

void juju::ui::refresh_button_action(button_state bs)
{
	switch (bs) {
	    case button_state::rest:
	    {
		    return;
	    }

	    case button_state::pressed:
	    {
		    MessageBox(nullptr, L"Refresh Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
		    break;
	    }

	    case button_state::released:
	    {
		    break;
	    }

	    case button_state::holding:
	    {
		    break;
	    }

	default:
		break;
	}
}

void juju::ui::front_listbox_action(listbox_commands lc) {
    switch (lc) {
		case listbox_commands::error_space:
		{
			// Not enough memory to store an item.
			break;
		}
		
		case listbox_commands::double_click:
		{
			// User double - clicked an item.
			break;
		}

		case listbox_commands::select_cancel:
		{
			// Selection was canceled(multi - selection mode).
			break;
		}

		// User selected a new item.
		case listbox_commands::select_change:
		{
			juju_codes code;
			string selected_text = m_front_lb.get_selection(&code);
			output_code(code);
			MessageBox(nullptr, selected_text.c_str(), ROS("Selected Item"), MB_OK);
			break;
		}

		case listbox_commands::lost_focus:
		{
			// List box lost focus.
			break;
		}

		case listbox_commands::gained_focus:
		{
			// List box gained focus.
			break;
		}

    default:
        // Handle unknown command
        break;
    } // end of switch(lc)
}

void juju::ui::lb_label_action(label_commands command)
{
	switch (command) {
		case label_commands::clicked:
		{
			MessageBox(nullptr, ROS("LABEL CLICKED"), ROS("clicked."), MB_OK);
			break;
		}

		case label_commands::dbl_clicked:
		{
			break;
		}

		case label_commands::disable:
		{
			break;
		}

		case label_commands::enable:
		{
			break;
		}

		default:
			break;

	} // end of switch(command)
}
