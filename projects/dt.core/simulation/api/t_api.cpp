#include "t_api.hpp"

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
#include CORE_TAPI_INCLUDE_PATH

core::codes test_api::create_files(const std::filesystem::path& directory, std::size_t number_of_files, std::size_t size_of_each_file_in_bytes)
{
	if (std::filesystem::is_directory(directory) == false) {
		return core::codes::invalid_directory_path;
	}
	
	
	std::size_t file_number = 0;
	for (std::size_t i = 0; i < number_of_files; i++) {

		std::filesystem::path new_file_path(directory / std::string("f" + std::to_string(file_number) + ".txt"));
		std::fstream file(new_file_path, std::ios::out | std::ios::trunc);
		if (file.is_open()) {
			// fill with data
			while (std::filesystem::file_size(new_file_path) < size_of_each_file_in_bytes) {
				file << "abc" << '\n';
			}
			file.close();
		}
		else {
			api::output_cp(api::match_code(core::codes::file_open_fail));
		}
		file_number++;
	}
	api::output_message(std::to_string(file_number) + " files were created in: " + directory.string() + '\n');


	return core::codes::success;
}

core::codes test_api::clear_directory(const std::filesystem::path& directory)
{
	if (std::filesystem::is_directory(directory) == false) {
		return core::codes::invalid_directory_path;
	}

	std::uintmax_t removed = 0;
	try {
		for (auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
			if (std::filesystem::remove(entry.path()) == true) {
				removed++;
			}

			if (entry.is_directory() == true) {
				removed += std::filesystem::remove_all(entry.path());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e){
		api::output_fse(e);
		return core::codes::std_filesystem_exception_caught;
	}
	catch (...) {
		api::output_cp(api::match_code(core::codes::unknown_exception_caught));
		return core::codes::unknown_exception_caught;
	}
	api::output_message(std::to_string(removed) + " files removed from: " + directory.string() + '\n');
	return core::codes::success;
}
