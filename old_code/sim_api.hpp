/**********************************************************/
//
// File: sim_api.hpp
//
// Purpose: simulate work and test system
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_MAIN_ENTRY_INCLUDE_PATH


namespace core {
	namespace test_api {
		/*
			tests the entire gui entry system for a duration(ms)

			floods the system with data and checks how it handles it
		*/
		codes full_system_test(std::size_t duration);
	}
}

