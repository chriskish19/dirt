/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#pragma once

#include CORE_NAMES_INCLUDE
#include CORE_CODES_INCLUDE_PATH
#include CORE_STL_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH

namespace test_api {
	/*
		creates a number of files at path directory with data
	*/
	core::codes create_files(const std::filesystem::path& directory, std::size_t number_of_files,std::size_t size_of_each_file_in_bytes);


	/*
		deletes all files in the given directory
	*/
	core::codes clear_directory(const std::filesystem::path& directory);
}