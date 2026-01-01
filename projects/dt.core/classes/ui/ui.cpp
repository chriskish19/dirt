#include "ui.hpp"

/***********************************************
*
* File: ui.cpp
*
* Purpose: ui.hpp definitions
*
* Project: dt.core
*
************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_UI_INCLUDE_PATH

core::gui::main_window_ui::main_window_ui(HWND window, HMENU menu, HINSTANCE hinst, LPVOID lp)
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

	// progress bar 1
	m_progress_bar1_d.window = window;
	m_progress_bar1_d.menu = (HMENU)window_ids::progress_bar1;
	m_progress_bar1_d.hinst = hinst;
	m_progress_bar1_d.lpParam = lp;
	m_progress_bar1_d.bar_caller = nullptr; // no caller for progress bars

	m_progress_bar1 = bar(m_progress_bar1_d);
	m_progress_bar1.create();

	// progress bar 2
	m_progress_bar2_d.window = window;
	m_progress_bar2_d.menu = (HMENU)window_ids::progress_bar2;
	m_progress_bar2_d.hinst = hinst;
	m_progress_bar2_d.lpParam = lp;
	m_progress_bar2_d.bar_caller = nullptr; // no caller for progress bars

	m_progress_bar2 = bar(m_progress_bar2_d);
	m_progress_bar2.create();

	// new button
	m_new_bd.window = window;
	m_new_bd.menu = (HMENU)window_ids::b_new;
	m_new_bd.hinst = hinst;
	m_new_bd.lpParam = lp;
	m_new_bd.button_caller = [this](button_state bs) {
		this->new_button_action(bs);
		};

	m_new_b = button(m_new_bd);
	m_new_b.create();
	HBITMAP hnewBmp = (HBITMAP)LoadImage(
		NULL,
		DT_NEW_BUTTON_PATH,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE);
	SendMessage(m_new_b.get_button_handle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hnewBmp);

	// open button
	m_open_bd.window = window;
	m_open_bd.menu = (HMENU)window_ids::b_open;
	m_open_bd.hinst = hinst;
	m_open_bd.lpParam = lp;
	m_open_bd.button_caller = [this](button_state bs) {
		this->open_button_action(bs);
		};

	m_open_b = button(m_open_bd);
	m_open_b.create();
	HBITMAP hopenBmp = (HBITMAP)LoadImage(
		NULL,
		DT_OPEN_BUTTON_PATH,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE);
	SendMessage(m_open_b.get_button_handle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hopenBmp);

	// save button
	m_save_bd.window = window;
	m_save_bd.menu = (HMENU)window_ids::b_save;
	m_save_bd.hinst = hinst;
	m_save_bd.lpParam = lp;
	m_save_bd.button_caller = [this](button_state bs) {
		this->save_button_action(bs);
		};

	m_save_b = button(m_save_bd);
	m_save_b.create();
	HBITMAP hsaveBmp = (HBITMAP)LoadImage(
		NULL,
		DT_SAVE_BUTTON_PATH,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE);
	SendMessage(m_save_b.get_button_handle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hsaveBmp);

	// copy button
	m_copy_bd.window = window;
	m_copy_bd.menu = (HMENU)window_ids::b_copy;
	m_copy_bd.hinst = hinst;
	m_copy_bd.lpParam = lp;
	m_copy_bd.button_caller = [this](button_state bs) {
		this->copy_button_action(bs);
		};

	m_copy_b = button(m_copy_bd);
	m_copy_b.create();
	HBITMAP hcopyBmp = (HBITMAP)LoadImage(
		NULL,
		DT_COPY_BUTTON_PATH,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE);
	SendMessage(m_copy_b.get_button_handle(), BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hcopyBmp);
}
void core::gui::main_window_ui::front_button_action(button_state bs)
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

void core::gui::main_window_ui::refresh_button_action(button_state bs)
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

void core::gui::main_window_ui::front_listbox_action(listbox_commands lc) {
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
			core::codes code;
			string selected_text = m_front_lb.get_selection(&code);
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

void core::gui::main_window_ui::lb_label_action(label_commands command)
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

void core::gui::main_window_ui::new_button_action(button_state bs)
{
	switch (bs) {
	case button_state::rest:
	{
		return;
	}

	case button_state::pressed:
	{
		MessageBox(nullptr, L"New Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
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

void core::gui::main_window_ui::open_button_action(button_state bs)
{
	switch (bs) {
	case button_state::rest:
	{
		return;
	}

	case button_state::pressed:
	{
		MessageBox(nullptr, L"Open Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
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

void core::gui::main_window_ui::save_button_action(button_state bs)
{
	switch (bs) {
	case button_state::rest:
	{
		return;
	}

	case button_state::pressed:
	{
		MessageBox(nullptr, L"Save Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
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

void core::gui::main_window_ui::copy_button_action(button_state bs)
{
	switch (bs) {
	case button_state::rest:
	{
		return;
	}

	case button_state::pressed:
	{
		MessageBox(nullptr, L"Copy Button Clicked!", L"Notification", MB_OK | MB_ICONINFORMATION);
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

core::gui::system_log_window_ui::system_log_window_ui(HWND window, HMENU menu, HINSTANCE hinst, LPVOID lp)
{
	
	m_cmdinputbar_td.window = window;
	m_cmdinputbar_td.menu = (HMENU)window_ids::cmdline;
	m_cmdinputbar_td.hinst = hinst;
	m_cmdinputbar_td.lpParam = lp;
	m_cmdinputbar_td.textbox_caller = [this](textbox_commands command) {
		this->cmdinputbar_action(command);
		};

	m_cmdinputbar_tb = textbox(m_cmdinputbar_td);
	m_cmdinputbar_tb.create();


	m_output_td.window = window;
	m_output_td.menu = (HMENU)window_ids::cmdline;
	m_output_td.hinst = hinst;
	m_output_td.lpParam = lp;
	m_output_td.textbox_caller = [this](textbox_commands command) {
		this->output_action(command);
		};

	m_output_tb = textbox(m_output_td);
	m_output_tb.create();
}

void core::gui::system_log_window_ui::cmdinputbar_action(textbox_commands command)
{
	switch (command)
	{

	case textbox_commands::change:  
		break;

	case textbox_commands::update: 
		break;

	case textbox_commands::set_focus:  
		break;

	case textbox_commands::kill_focus:  
		break;

	case textbox_commands::error_out_of_memory:  
		break;

	case textbox_commands::scroll_horizontal:  
		break;

	case textbox_commands::vertical_scroll:  
		break;

	case textbox_commands::max_length:  
		break;

	case textbox_commands::left_aligned:  
		break;

	case textbox_commands::right_aligned: 
		break;

	default:
		// Unhandled or unknown notification code
		break;
	}
}

void core::gui::system_log_window_ui::output_action(textbox_commands command)
{
	switch (command)
	{

	case textbox_commands::change:
		break;

	case textbox_commands::update:
		break;

	case textbox_commands::set_focus:
		break;

	case textbox_commands::kill_focus:
		break;

	case textbox_commands::error_out_of_memory:
		break;

	case textbox_commands::scroll_horizontal:
		break;

	case textbox_commands::vertical_scroll:
		break;

	case textbox_commands::max_length:
		break;

	case textbox_commands::left_aligned:
		break;

	case textbox_commands::right_aligned:
		break;

	default:
		// Unhandled or unknown notification code
		break;
	}
}
