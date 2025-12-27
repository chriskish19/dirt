
/***************************************
*  File: file.cpp
*
*  Purpose: file.hpp definitions
*
*  Project: dt.core
* *************************************/

#include CORE_NAMES_INCLUDE
#include CORE_FILELOG_INCLUDE_PATH

core::file::file(const fs::path& folder, const std::string& file_name)
{
	m_out_file_path = folder / file_name;
}

core::codes core::file::load()
{
	out_file.open(m_out_file_path, std::ios::out | std::ios::trunc);

	if (!out_file.is_open()) {
		// Handle the error if the file couldn't be opened
		std::cerr << "Failed to open file: " << m_out_file_path << std::endl;
		return codes::file_open_fail;
	}

	return codes::success;
}

core::codes core::file::write_buffer(std::vector<log>* buffer)
{
	
	std::string header = api::time_now("FULL BUFFER DUMP!");

	try {
		out_file << header << '\n';
	}
	catch (const std::ios_base::failure& e) {
		// Handle the write error
		std::cerr << "Write error: " << e.what() << std::endl;
		return codes::write_to_file_fail;
	}
	
	for (auto& log : *buffer) {
		auto& message = *log.message;
		
		try {
			out_file << message;
		}
		catch (const std::ios_base::failure& e) {
			// Handle the write error
			std::cerr << "Write error: " << e.what() << std::endl;
			return codes::write_to_file_fail;
		}
	}
	
	std::string footer = api::time_now("END OF BUFFER DUMP!");

	try {
		out_file << footer << '\n';
	}
	catch (const std::ios_base::failure& e) {
		// Handle the write error
		std::cerr << "Write error: " << e.what() << std::endl;
		return codes::write_to_file_fail;
	}


	return codes::success;
}

core::codes core::file::write_message(const std::string& message)
{
	try {
		out_file << message;
	}
	catch (const std::ios_base::failure& e) {
		// Handle the write error
		std::cerr << "Write error: " << e.what() << std::endl;
		return codes::write_to_file_fail;
	}
	return codes::success;
}
