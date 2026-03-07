#include "drawing.hpp"
/**********************************************************/
//
// File: drawing.cpp
//
// Purpose: drawing.hpp definitions
//
// Project: dt
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_DRAWING_INCLUDE_PATH


void core::main::loading_bar::draw() {
	// Get pointer to the start of the current 3-byte sequence
	const char8_t* current_frame_start = frames + (frame_index * bytes_per_frame);
	std::cout << "\r" << BLUE;
	std::cout.write(reinterpret_cast<const char*>(current_frame_start), bytes_per_frame);
	std::cout << m_message << DEFAULT_COLOR;
	// Advance to next frame
	frame_index = (frame_index + 1) % num_frames;
}

void core::main::window_loading_bar::draw() {
	wchar_t next = m_frames[frame_index];
	SetWindowText(m_handle, std::wstring(next + m_message).c_str());
	frame_index = (frame_index + 1) % num_frames;
}
