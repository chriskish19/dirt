/***********************************************
* 
* File: listbox.hpp
*
* Purpose: handle listboxes in a win32 window.
*
* Project: jujubee
*
************************************************/

#pragma once
#include NAMES_INCLUDE
#include DEPENDENCIES_INCLUDE_PATH
#include CODES_INCLUDE_PATH

#define LB_MAX_STRING 256

namespace juju {
    enum class listbox_commands {
        none = 0,

        // Adding & Removing Items
        add_string = LB_ADDSTRING,            // Adds a string to the list box.
        insert_string = LB_INSERTSTRING,      // Inserts a string at a specific index.
        delete_string = LB_DELETESTRING,      // Removes an item at a specified index.
        reset_content = LB_RESETCONTENT,      // Removes all items from the list box.

        // Selection Handling
        get_cursel = LB_GETCURSEL,            // Gets the index of the currently selected item (single-selection list).
        set_cursel = LB_SETCURSEL,            // Sets the selection to a specific index (single-selection list).
        get_sel = LB_GETSEL,                  // Gets selection state of an item (multi-selection list).
        set_sel = LB_SETSEL,                  // Selects/Deselects an item (multi-selection list).
        get_selcount = LB_GETSELCOUNT,        // Gets the number of selected items (multi-selection list).
        get_selitems = LB_GETSELITEMS,        // Gets an array of selected items (multi-selection list).

        // Retrieving & Modifying Items
        get_text = LB_GETTEXT,                  // Gets the text of an item.
        get_textlen = LB_GETTEXTLEN,            // Gets the length of an item's text.
        set_itemdata = LB_SETITEMDATA,          // Sets application-defined data for an item.
        get_itemdata = LB_GETITEMDATA,          // Retrieves application-defined data for an item.
        find_string = LB_FINDSTRING,            // Finds an item that starts with a specified string.
        find_stringexact = LB_FINDSTRINGEXACT,  // Finds an exact string match.

        // Size & Appearance
        set_horizontalextent = LB_SETHORIZONTALEXTENT,  // Sets horizontal scroll width.
        get_horizontalextent = LB_GETHORIZONTALEXTENT,  // Gets horizontal scroll width.
        set_columnwidth = LB_SETCOLUMNWIDTH,            // Sets the width of columns in a multi-column list box.
        set_itemheight = LB_SETITEMHEIGHT,              // Sets the height of an item (for owner-drawn list boxes).
        get_itemheight = LB_GETITEMHEIGHT,              // Gets the height of an item.

        // Miscellaneous
        set_locale = LB_SETLOCALE,              // Sets the locale for sorting.
        get_locale = LB_GETLOCALE,              // Gets the locale for sorting.
        get_count = LB_GETCOUNT,                // Gets the number of items in the list box.
        dir = LB_DIR,                           // Adds files matching a specified mask (e.g., *.txt).

        // Extended Features
        set_caretindex = LB_SETCARETINDEX,      // Sets focus to a specific item.
        get_caretindex = LB_GETCARETINDEX,      // Gets the index of the focused item.
        set_topindex = LB_SETTOPINDEX,          // Scrolls the list so a specified item is at the top.
        get_topindex = LB_GETTOPINDEX,          // Gets the index of the topmost visible item.

        // command messages from win32 proc
        error_space = LBN_ERRSPACE,	            // Not enough memory to store an item.
        double_click = LBN_DBLCLK,	            // User double - clicked an item.
        select_cancel = LBN_SELCANCEL,	        // Selection was canceled(multi - selection mode).
        select_change = LBN_SELCHANGE,          // User selected a new item.
        lost_focus = LBN_KILLFOCUS,	            // List box lost focus.
        gained_focus = LBN_SETFOCUS	            // List box gained focus.
    };

	struct listbox_description {
		string class_name = ROS("LISTBOX");
		string window_name = ROS("LB1");
		DWORD style_flags = 0;
		std::size_t xPos = 0;
		std::size_t yPos = 0;
		std::size_t width = 0;
		std::size_t height = 0;
		HWND window = nullptr;
		HMENU menu = nullptr;
		HINSTANCE hinst = GetModuleHandle(NULL);
		LPVOID lpParam = nullptr;
		std::function<void(listbox_commands)> listbox_caller = nullptr;
	};
	
	
	class listbox {
	public:
		listbox() = default;
        listbox(listbox_description lbd);

        juju_codes create();

        juju_codes add_string(const string& list_message);
        string get_selection(juju_codes* code);

        virtual void action(listbox_commands command);

        listbox_description get_lbd() { return m_lbd; }
        HWND get_listbox_handle() { return m_lb_handle; }
        listbox_commands get_command() { return m_command; }
	protected:
        listbox_description m_lbd = {};
        HWND m_lb_handle = nullptr;
        listbox_commands m_command = listbox_commands::none;
	};
}