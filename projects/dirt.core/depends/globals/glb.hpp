/**********************************************************/
//
// File: glb.hpp
//
// Purpose: global objects
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_STL_INCLUDE_PATH

// global terminal
#include CORE_TOUT_INCLUDE_PATH
namespace global {
	extern std::unique_ptr<core::t_out> terminal;
}