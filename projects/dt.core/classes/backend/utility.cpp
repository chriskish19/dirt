#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#if! UNDER_CONSTRUCTION

/**********************************************************/
//
// File:
//
// Purpose:
//
// Project:
//
/**********************************************************/

#include CORE_NAMES_INCLUDE
#include CORE_UTILITY_INCLUDE_PATH

void core::utility::output_em(const code_pkg cp, const std::string location)
{
	std::string message = cp.m_s_code + '\n'
		+ location + '\n';

}

std::uintmax_t core::utility::file_numbers(const std::filesystem::path& p) {
	// returns 0 on a number of problems, if it does not exist and its not a directory 
	// it will return 0 and no errors will be thrown.
	// only counts regular files, nothing else.
	try {
		std::uintmax_t count = 0;
		if (std::filesystem::exists(p) && std::filesystem::is_directory(p)) {
			for (const auto& entry : std::filesystem::directory_iterator(p)) {
				if (entry.is_regular_file()) {
					++count;
				}
			}
		}
		return count;
	}
	catch (const std::filesystem::filesystem_error& e) {
		output_em(std_filesystem_exception_caught_pkg);
		output_fse(e);
	}
	catch (...) {
		output_em(unknown_exception_caught_pkg);
	}

	// exception thrown, error occured so return 0
	return 0;
}

#endif