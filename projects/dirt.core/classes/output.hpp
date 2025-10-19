/**********************************************************/
//
// File: output.hpp
//
// Purpose: handle outputting to the gui or terminal
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH

#include <Windows.h>

namespace core {
	codes main_output(const std::string& log);
}