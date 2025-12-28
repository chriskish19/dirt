/**********************************************************/
//
// File: file.hpp (file logger)
//
// Purpose: send logs to a file
//
// Project: dt.core
//
/**********************************************************/

#pragma once
#include CORE_NAMES_INCLUDE
#include CORE_DEFINES_INCLUDE_PATH
#include CORE_CODES_INCLUDE_PATH
#include CORE_BASE_INCLUDE_PATH
#include CORE_API_INCLUDE_PATH

namespace core {
	namespace logger {
		class file {
		public:
			file() = default;
			file(const fs::path& folder, const std::string& file_name);
			codes load();													// opens file for writing
			codes write_buffer(std::vector<log>* buffer);					// writes a log buffer to out_file
			codes write_message(const std::string& message);				// write a single message to out_file
		protected:
			fs::path m_out_file_path;										// full path to out file
			std::ofstream out_file;											// output file stream file object
		};
	}
}