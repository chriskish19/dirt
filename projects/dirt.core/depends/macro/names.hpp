/**********************************************************/
//
// File: names.hpp
//
// Purpose: easy file moves/ name changes
//
// Project: dirt.core
//
/**********************************************************/

#pragma once

// dependencies
#define CORE_STL_PRIVATE_INCLUDE_PATH			CORE_stl_includes_hpp				// the entire stl library
#define CORE_STL_INCLUDE_PATH					CORE_stl_hpp						// specific version include stl library

// codes
#define CORE_CODES_INCLUDE_PATH					CORE_codes_hpp						// error codes
#define CORE_ARGS_INCLUDE_PATH					CORE_args_hpp						// cmdline arguments

// api
#define CORE_API_INCLUDE_PATH					CORE_dirt_api_hpp					// helper functions

// classes
#define CORE_PROCESSOR_INCLUDE_PATH				CORE_processor_hpp					// processes files
#define CORE_CMDLINE_INCLUDE_PATH				CORE_cmdline_hpp					// processes cmd line args
#define CORE_LOGGER_INCLUDE_PATH				CORE_logger_hpp						// logger queue
#define CORE_TOUT_INCLUDE_PATH					CORE_t_out_hpp						// terminal outputing

// simulation
#define CORE_SIM_API_INCLUDE_PATH				CORE_sim_api_hpp					// simulation api
#define CORE_SIM_INCLUDE_PATH					CORE_sim_hpp						// sim classes

// macros
#define CORE_DEFINES_INCLUDE_PATH				CORE_defines_hpp					// macros

// globals
#define CORE_GLB_INCLUDE_PATH					CORE_glb_hpp						// global objects