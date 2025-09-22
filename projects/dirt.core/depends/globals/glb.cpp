/**********************************************************/
//
// File: glb.cpp
//
// Purpose: glb.hpp linked objects
//
// Project: dirt.core
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_GLB_INCLUDE_PATH

namespace global {
	std::unique_ptr<core::t_out> terminal = nullptr;
}